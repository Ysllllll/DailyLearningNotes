#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <string>

ssize_t readn(int fd, void *buf, size_t n);
ssize_t writen(int fd, void *buf, size_t n);

void handle_for_sigpipe();
int setSocketNonBlocking(int fd);
int socket_bind_listen();
#endif