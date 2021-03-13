
#include "Util.h"
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

ssize_t readn(int fd, void *buf, size_t n)
{
	size_t surplusRead = n;
	size_t totalRead = 0;
	size_t onceRead = 0;

	char *ptr = (char *)buf;

	while (surplusRead > 0)
	{
		if (onceRead = read(fd, ptr, surplusRead) < 0)
		{
			if (errno == EINTR)
				onceRead = 0;
			else if (errno == EAGAIN)
				return totalRead;
			else
				return -1;
		}
		else if (onceRead == 0)
			return totalRead;
		else
		{
			totalRead += onceRead;
			surplusRead -= onceRead;
			ptr += onceRead;
		}
	}
	return totalRead;
}

ssize_t writen(int fd, void *buf, size_t n)
{
	size_t surplusWrite = n;
	size_t totalWrite = 0;
	size_t onceWrite = 0;
	char *ptr = (char *)buf;

	while (surplusWrite > 0)
	{
		if ((onceWrite = write(fd, buf, surplusWrite)) < 0)
		{
			if (errno == EINTR)
				onceWrite = 0;
			else if (errno == EAGAIN)
				return totalWrite;
			else
				return -1;
		}
		else
		{
			totalWrite += onceWrite;
			surplusWrite -= onceWrite;
			ptr += onceWrite;
		}
	}
	return totalWrite;
}

void handle_for_sigpipe()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGPIPE, &sa, NULL);
}

int setSocketNonBlocking(int fd)
{
	int flag;
	if ((flag = fcntl(fd, F_GETFL, 0)) == -1)
		return -1;
	if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1)
		return -1;
	return 0;
}