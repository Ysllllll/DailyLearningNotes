#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define BUFFERSIZE 10
#define DPS 10
#define BURST 100

static volatile int token = 0;

static void alrm_handler(int s)
{
	alarm(1);
	token++;
	if (token > BURST)
		token--;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		perror("Usage...");
		exit(0);
	}

	signal(SIGALRM, alrm_handler);
	alarm(1);

	char buf[BUFFERSIZE];
	int len, ret, pos;
	int sfd, dfd = 1;

	do
	{
		sfd = open(argv[1], O_RDONLY);
		if (sfd < 0)
		{
			if (errno != EINTR)
			{
				perror("open()");
				exit(1);
			}
		}
	} while (sfd < 0);

	while (1)
	{
		while (token <= 0)
			pause();
		token--;

		while ((len = read(sfd, buf, BUFFERSIZE)) < 0)
		// 被阻塞了 3s，token = 3；写了3次10个字节
		{
			if (errno == EINTR)
				continue;
			perror("read()");
			exit(1);
		}
		if (len == 0)
			break;

		pos = 0;
		while (len > 0)
		{
			ret = write(dfd, buf + pos, len);
			if (ret < 0)
			{
				if (errno == EINTR)
					continue;
				perror("write()");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}
	}

	close(sfd);
	exit(0);
}