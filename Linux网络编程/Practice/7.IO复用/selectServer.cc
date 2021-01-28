#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set readfds, testfds;
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0); //建立服务器端socket
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8000);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);
    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds); //将服务器端socket加入到集合中
    while (1)
    {
        //与blockServer不同，这里是一个线程管理多个连接，而不是一个线程管理一个连接
        //使用select来管理多个连接的缺点是：
        //   1.这一个线程能够管理的最大连接数有限制，通常为1024
        //   2.每次调用select，都需要把fd集合从用户态拷贝到内核态，这个开销在fd很多时很大
        //   3.对socket线性扫描，采用轮询的方式，效率低

        int fd;
        int nread;
        testfds = readfds;
        printf("server waiting\n");

        /*无限期阻塞，并测试文件描述符变动 */
        // 1024个文件描述符
        result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);
        if (result < 1)
        {
            printf("select error");
        }

        /*扫描所有的文件描述符*/
        for (fd = 0; fd < FD_SETSIZE; fd++)
        {
            /*找到相关文件描述符*/
            if (FD_ISSET(fd, &testfds))
            {
                /*判断是否为服务器套接字，是则表示为客户请求连接。*/
                if (fd == server_sockfd)
                {
                    client_len = sizeof(client_address);
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
                    FD_SET(client_sockfd, &readfds); //将客户端socket加入到集合中
                    printf("connected client:%d\n", client_sockfd);
                }
                /*客户端socket中有数据请求时*/
                else
                {
                    char buff[4096];
                    memset(buff, 0, 4096);
                    int nread = recv(fd, buff, 4096, 0);

                    /*客户数据请求完毕，关闭套接字，从集合中清除相应描述符 */
                    if (nread == 0)
                    {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("removing client on fd %d\n", fd);
                    }
                    /*处理客户数据请求*/
                    else
                    {
                        printf("recv msg from client connectfd: %d, content:%s\n", fd, &buff);
                        char *msg = "server test";
                        send(fd, msg, strlen(msg), 0);
                    }
                }
            }
        }
    }
}