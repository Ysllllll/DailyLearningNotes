
#include "Util.h"

ssize_t readn(int fd, void *buf, size_t n)
{
	size_t surplusRead = n;
	size_t totalRead = 0;
	size_t onceRead = 0;

	char *ptr = (char *)buf;

	while (surplusRead > 0)
	{
		if ((onceRead = read(fd, ptr, surplusRead)) < 0)
		{
			if (errno == EINTR)
				onceRead = 0;
			else if (errno == EAGAIN)
				return totalRead;
			else
			{
				return -1;
			}
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

void handle_for_sigpipe() //考虑为什么
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

int socket_bind_listen(int port, char *ip)
{
	if (port < 0 || port > 65535)
		return -1;

	int listen_fd = 0;
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return -1;

	int optval = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		close(listen_fd);
		return -1;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //后序该如何重新设定？？
	inet_pton(AF_INET, ip, &server_addr.sin_addr);
	server_addr.sin_port = htons((unsigned int)port);

	if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		close(listen_fd);
		return -1;
	}
	if (listen(listen_fd, 2048) == -1)
	{
		close(listen_fd);
		return -1;
	}

	return listen_fd;
}