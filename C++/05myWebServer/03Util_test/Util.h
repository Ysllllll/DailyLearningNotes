#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

ssize_t readn(int fd, void *buf, size_t n);
ssize_t writen(int fd, void *buf, size_t n);

void handle_for_sigpipe();
int setSocketNonBlocking(int fd);
int socket_bind_listen(int port, char *ip);
#endif