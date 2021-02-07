#include <errno.h>
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

#define LISTEN_PORT 8000
#define MAX_BYTES_LINE 4096

int main(int argc, char **argv)
{

    int sockfd, connectfd;
    int dataLen = 0;
    int i_listenPort = 8000;

    char buffer[MAX_BYTES_LINE];
    char *c_ipAddr = "127.0.0.1";
    char *c_responseInfo = "Hello,you are connected!\n";
    struct sockaddr_in serverAddr;

    // socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create server socket error:%s(errno:%d)\n", strerror(errno), errno);
        exit(0);
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //auto get local server ip addr
    serverAddr.sin_port = htons(LISTEN_PORT);

    // bind
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        printf("bind socket error:%s(errno:%d)\n", strerror(errno), errno);
        exit(0);
    }

    // listen
    if (listen(sockfd, 10) == -1)
    {
        printf("listen socket error :%s(errno:%d)\n", strerror(errno), errno);
        exit(0);
    }
    printf("waiting for client to connect\n");
    while (1)
    {
        //本段程序主要描述的展示的是：同步阻塞IO模型 下服务端采用 多线程 的运行模式，相当于一个连接一个线程
        //该服务器每次accept一个客户端后，都会等待接收recv该客户端信息并send一个信息，
        //此过程中若有新的客户端连接过来会被阻塞
        //运行：g++ blockServer.cc -o blockServer
        //     ./blockServer
        //     新开cmd1：telnet 192.168.124.7 8000 作为客户端1
        //     新开cmd2：telnet 192.168.124.7 8000 作为客户端2，如果客户端1连接后没有发送信息，服务器接收该连接的accept将被阻塞

        // accept
        if ((connectfd = accept(sockfd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            printf("accept socket error:%s(errorno:%d)", strerror(errno), errno);
            continue;
        }
        printf("connected client:%d\n", connectfd);

        // recv
        dataLen = recv(connectfd, buffer, MAX_BYTES_LINE, 0);
        if (!fork())
        { //in sub process ,fork() will return 0
            // send
            if (send(connectfd, c_responseInfo, strlen(c_responseInfo), 0) == -1)
            {
                perror("send response error");
                close(connectfd);
                exit(0);
            }
            buffer[dataLen] = '\0';
            printf("recv msg from client connectfd: %d, content:%s\n", connectfd, buffer);
            close(connectfd); // close connection
        }
    }

    close(sockfd);
}