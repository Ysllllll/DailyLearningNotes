#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    // 1.转换传入服务器参数
    if (argc <= 2)
    {
        printf("error, arrgument amount error");
        return 1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    // 2.服务器参数指定
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;

    // 3.分配服务器资源
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(sock, 5);
    assert(ret != -1);

    // 4.连接客户端并获取客户端地址
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int conn = accept(sock, (struct sockaddr *)&client, &client_addrlength);
    assert(conn != -1);
    char newMsgIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client.sin_addr, newMsgIp, INET_ADDRSTRLEN);

    // 5.分配接收缓存和发送缓存
    char recv_buffer[BUF_SIZE];
    char send_buffer[100];

    // 6.服务器接收客户端的信息，并返回给客户端特定的信息
    while (1)
    {
        memset(recv_buffer, '\0', BUF_SIZE);
        recv(conn, recv_buffer, BUF_SIZE - 1, MSG_DONTWAIT);

        if (recv_buffer[0] != '\0')
            printf("recv msg from client %s: %s\n", newMsgIp, recv_buffer);

        memset(send_buffer, '\0', strlen(send_buffer));
        strcat(send_buffer, "server info");
        send(conn, send_buffer, BUF_SIZE - 1, 0);
    }
    return 0;
}
