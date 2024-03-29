#include "util.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

ssize_t readn(int fd, void *buff, size_t n)
{
	size_t nleft = n;
	ssize_t nread = 0;
	ssize_t readSum = 0;
	char *ptr = (char *)buff;
	while (nleft > 0)
	{
		if ((nread = read(fd, ptr, nleft)) < 0)
		{
			if (errno == EINTR) //表示中断
				nread = 0;
			else if (errno == EAGAIN) //表示未读完
			{
				return readSum;
			}
			else //否则，出错
			{
				return -1;
			}
		}
		else if (nread == 0) //表示读取结束
			break;
		readSum += nread;
		nleft -= nread;
		ptr += nread;
	}
	return readSum;
}

ssize_t writen(int fd, void *buff, size_t n)
{
	size_t nleft = n;
	ssize_t nwritten = 0;
	ssize_t writeSum = 0;
	char *ptr = (char *)buff;
	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0)
			{
				if (errno == EINTR || errno == EAGAIN) //写的过程中出现中断或还不能写，重试
				{
					nwritten = 0;
					continue;
				}
				else //否则，出错
					return -1;
			}
		}
		writeSum += nwritten;
		nleft -= nwritten;
		ptr += nwritten;
	}
	return writeSum;
}

void handle_for_sigpipe()
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGPIPE, &sa, NULL))
		return;
}

int setSocketNonBlocking(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (flag == -1)
		return -1;

	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) == -1)
		return -1;
	return 0;
}