#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    // ./server 192.168.124.x 54321 text.txt
    // telnet 192.168.124.x 54321
    if (argc <= 3)
    {
        printf("Wrong number of parameters\n");
        return 1;
    }

    //服务端参数指定
    char *ip = argv[1];
    int port = atoi(argv[2]);
    const char *file_name = argv[3];

    //打开请求的文件
    int filefd = open(file_name, O_RDONLY);
    assert(filefd > 0);
    struct stat stat_buf;
    fstat(filefd, &stat_buf);

    //服务器参数指定
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    //服务器套接字资源分配
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    int retu = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    assert(retu != -1);
    retu = listen(sockfd, 5);
    assert(sockfd != -1);

    //接受客户端连接
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sockfd, (struct sockaddr *)&client, &client_addrlength);
    if (connfd >= 0)
    {

        sendfile(connfd, filefd, NULL, stat_buf.st_size);
        close(connfd);
    }
    close(sockfd);
    return 0;
}
