#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
//#include <curses.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    // 1.转换传入的客户端参数
    if (argc <= 2)
    {
        printf("error, argue is %d\n", argc);
        return 1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    // 2.客户端参数指定
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    // 3.客户端套接字资源分配
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    int conn = connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    assert(conn != -1);
    char newMsgIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_address.sin_addr, newMsgIp, INET_ADDRSTRLEN);

    // 4.分配接收缓存与发送缓存
    char recv_buffer[1024];
    char send_buffer[BUFF_SIZE];

    // 5.服务器接收客户端的信息，并返回给客户端特定的信息
    while (1)
    {
        memset(recv_buffer, '\0', BUFF_SIZE);
        recv(sockfd, recv_buffer, BUFF_SIZE - 1, MSG_DONTWAIT);

        if (recv_buffer[0] != '\0')
            printf("recv from server %s: %s\n", newMsgIp, recv_buffer);

        // scanf("%s", send_buffer);
        sleep(1);
        memset(send_buffer, '\0', BUFF_SIZE);
        strcpy(send_buffer, "client info");
        send(sockfd, send_buffer, BUFF_SIZE - 1, 0);
    }
    close(sockfd);
    return 0;
}
