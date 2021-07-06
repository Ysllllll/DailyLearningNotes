#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("argument error\n");
        return 0;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    //指定服务器参数
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;

    //分配客户端接口资源
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    int conn = connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    assert(conn != -1);

    //生成待发送资源
    char send_buffer[BUFF_SIZE];
    memset(send_buffer, 0, BUFF_SIZE);
    strcpy(send_buffer, "GET \thttp://www.baidu.com/index.html \tHTTP/1.1\r\nHost: \twww.baidu.com\r\n\r\n");

    //发送并将发送的信息输出
    int send_len = send(sockfd, send_buffer, strlen(send_buffer), 0);
    printf("client send %d bytes success, info: %s\n", send_len, send_buffer);

    memset(send_buffer, 0, BUFF_SIZE);
    int recv_len = recv(sockfd, send_buffer, BUFF_SIZE, 0);
    printf("client recv %d bytes success, info: %s\n", strlen(send_buffer), send_buffer);
    close(sockfd);
}