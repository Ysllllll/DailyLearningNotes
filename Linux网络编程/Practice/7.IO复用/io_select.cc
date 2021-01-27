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
    int ret = bind(sockfd, (struct sockaddr *)&server, sizeof(server));
    assert(ret != -1);
    ret = listen(sockfd, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int conn = accept(sockfd, (struct sockaddr *)&client, &client_len);
    assert(conn >= 0);

    char buf[1024];
    fd_set read_fds;
    FD_ZERO(&read_fds);

    while (1)
    {
        memset(&buf, 0, 1024);
        FD_SET(conn, &read_fds);
        ret = select(conn + 1, &read_fds, NULL, NULL, NULL);
        assert(ret >= 0);

        if (FD_ISSET(conn, &read_fds))
        {
            ret = recv(conn, buf, 1024, 0);
            if (ret <= 0)
                break;
            printf("get %d bytes of normal data:%s", strlen(buf), buf);
        }
    }
    close(conn);
    close(sockfd);
    return 0;
}
