#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("argument error\n");
        return 0;
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    inet_pton(AF_INET, ip, &server.sin_addr);
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    int conn = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    if (conn == -1)
    {
        printf("error is %d, info:%s\n", errno, strerror(errno));
    }

    char buffer[1024];
    memset(buffer, 0, 1024);

    strcpy(buffer, "test select\n");
    send(sockfd, buffer, 1024, 0);
    close(sockfd);
    return 0;
}