#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    printf("sever socket: %d\n", sock);
    struct sockaddr_in client[5];
    socklen_t client_addrlength = sizeof(client[0]);
    int connfd[5] = {-1, -1, -1, -1};
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        connfd[i] = accept(sock, (struct sockaddr *)&(client[i]), &client_addrlength);
        if (connfd[i] < 0)
        {
            printf("errno is: %d\n", errno);
        }
        else
        {
            char remote[INET_ADDRSTRLEN];
            printf("connected with ip: %s and port: %d, connfd[%d] = %d\n",
                   inet_ntop(AF_INET, &client[i].sin_addr, remote, INET_ADDRSTRLEN), ntohs(client[i].sin_port), i, connfd[i]);
        }
    }
    for (i = 0; i < 5; i++)
        close(connfd[i]);

    close(sock);
    return 0;
}
