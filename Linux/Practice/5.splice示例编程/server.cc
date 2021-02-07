#define _GNU_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    //测试：
    // ./client 192.168.124.7 54321
    // telnet 192.168.124.7 54321

    // 1.转换输入的客户端参数
    if (argc <= 2)
    {
        printf("Wrong number of parameters!");
        return 1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    // 2.客户端参数指定
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    // 3.客户端套接字资源分配
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(sockfd, 5);
    assert(ret != -1);

    //等待客户端的连接
    struct sockaddr_in client;
    socklen_t client_length = sizeof(client);
    int connfd = accept(sockfd, (struct sockaddr *)&client, &client_length);
    if (connfd < 0)
    {
        printf("error\n");
        return 1;
    }
    else
    {
        while (1)
        {
            // 用管道连接起 socket的输入和输出 实现0拷贝
            int pipefd[2];
            assert(ret != -1);
            ret = pipe(pipefd);
            ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
            assert(ret != -1);
            ret = splice(pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
            assert(ret != -1);
        }
        close(connfd);
    }

    close(sockfd);
    return 0;
}
