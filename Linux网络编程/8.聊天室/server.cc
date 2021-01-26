#define _GNU_SOURCE 1
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#define USER_LIMIT 5   //最大用户数量
#define BUFFER_SIZE 64 //读缓冲区的大小
#define FD_LIMIT 65535 //文件描述符数量限制

//客户数据：客户端socket地址、待写到客户端的数据的位置、从客户端读入的数据
struct client_data
{
    sockaddr_in address;
    char *write_buff;
    char buf[BUFFER_SIZE];
};

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

int main(int argc, char *argv[])
{

    if (argc <= 2)
    {
        printf("Wrong number of parameters \n");
        return 1;
    }
    int port = atoi(argv[2]);
    char *ip = argv[1];

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(sockfd, 5);
    assert(ret != -1);

    //创建users数组，分配FD_LIMIT个client_data对象。可以预期：
    //每个可能的socket连接都可以获得一个这样的对象，并且socket的值可以直接
    //用来索引（作为数组的下标）socket连接对应的client_data对象，这是将
    //socket和客户数据关联的简单而高效的方式
    client_data *users = new client_data[FD_LIMIT];
    //尽管我们分配了足够多的client_data对象，但是为了提高poll的性能，仍然有必要限制用户的数量
    pollfd fds[USER_LIMIT + 1];
    int user_count = 0;
    for (int i = 0; i <= USER_LIMIT; i++)
    {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[0].fd = sockfd;
    fds[0].events = POLLIN | POLLERR;
    fds[0].revents = 0;

    while (1)
    {

        ret = poll(fds, user_count + 1, -1);
        if (ret < 0)
        {
            printf("poll failure\n");
            break;
        }

        for (int i = 0; i < user_count + 1; i++)
        {
            if ((fds[i].fd == sockfd) && (fds[i].revents & POLLIN))
            {

                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(sockfd, (struct sockaddr *)&client_address, &client_addrlength);

                if (connfd < 0)
                {
                    printf("errno is: %d\n", errno);
                    continue;
                }

                //如果请求太多，则关闭新到的连接
                if (user_count >= USER_LIMIT)
                {
                    const char *info = "to many users\n";
                    printf("%s", info);
                    send(connfd, info, strlen(info), 0);
                    close(connfd);
                    continue;
                }

                //对于新的连接，同时修改fds和users数组
                //users[connfd]对应于新连接文件描述符connfd的客户数据
                user_count++;
                users[connfd].address = client_address;
                setnonblocking(connfd);
                fds[user_count].fd = connfd;
                fds[user_count].events = POLLIN | POLLRDHUP | POLLERR;
                fds[user_count].revents = 0;
                printf("come a new client, now hava %d users\n", user_count);
            }
            else if (fds[i].revents & POLLERR)
            {
                printf("get an error from %d\n", fds[i].fd);
                char errors[100];
                memset(errors, '\0', 100);
                socklen_t length = sizeof(errors);

                if (getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, &errors, &length) < 0)
                {
                    printf("get socketopt failed\n");
                }
                continue;
            }
            else if (fds[i].revents & POLLRDHUP)
            {
                //如果客户端关闭连接，则服务器也关闭对应的连接，并将用户总数减1
                users[fds[i].fd] = users[fds[user_count].fd];
                close(fds[i].fd);
                fds[i] = fds[user_count];
                i--;
                user_count--;
                printf("a client left\n");
            }
            else if (fds[i].revents & POLLIN)
            {

                int connfd = fds[i].fd;
                memset(users[connfd].buf, '\0', BUFFER_SIZE);
                ret = recv(connfd, users[connfd].buf, BUFFER_SIZE - 1, 0);
                printf("get %d bytes of client data %s from %d\n", ret, users[connfd].buf, connfd);

                if (ret < 0)
                {
                    //如果读操作出错，则关闭连接
                    if (errno != EAGAIN)
                    {
                        close(connfd);
                        users[fds[i].fd] = users[fds[user_count].fd];
                        fds[i] = fds[user_count];
                        i--;
                        user_count--;
                    }
                }
                else if (ret == 0)
                {
                }
                else
                {
                    //如果接收到客户数据，则通知其他socket连接准备写数据
                    for (int j = 1; j <= user_count; j++)
                    {
                        if (fds[j].fd == connfd)
                        {
                            continue;
                        }

                        fds[j].events |= ~POLLIN;
                        fds[j].events |= POLLOUT;
                        users[fds[j].fd].write_buff = users[connfd].buf;
                    }
                }
            }
            else if (fds[i].revents & POLLOUT)
            {
                int connfd = fds[i].fd;
                if (!users[connfd].write_buff)
                {
                    continue;
                }
                ret = send(connfd, users[connfd].write_buff, strlen(users[connfd].write_buff), 0);

                // 写完数据后需要重新注册fds[i]上的可读事件
                users[connfd].write_buff = NULL;
                fds[i].events |= ~POLLOUT;
                fds[i].events |= POLLIN;
            }
        }
    }

    delete[] users;
    close(sockfd);
    return 0;
}
