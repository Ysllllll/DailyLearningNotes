#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("arguement error\n");
        return 1;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);

    struct sockaddr_in client;
    socklen_t client_addr_len = sizeof(client);

    while (1)
    {
        char buff[1024];
        memset(buff, '\0', 1024);
        recvfrom(sock, buff, BUFF_SIZE - 1, 0, (struct sockaddr *)&client, &client_addr_len);
        printf("recv info: %s from ip: %s:%d\n", buff, inet_ntop(AF_INET, &client.sin_addr, ip, INET_ADDRSTRLEN), ntohs(client.sin_port));
        sendto(sock, buff, BUFF_SIZE - 1, 0, (struct sockaddr *)&client, client_addr_len);
    }

    close(sock);
    return 0;
}