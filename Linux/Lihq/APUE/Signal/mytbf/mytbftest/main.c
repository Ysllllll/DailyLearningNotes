#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include "mytbf.h"

#define BUFFERSIZE 1024
#define CPS 10
#define BURST 100

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		perror("Usage...");
		exit(0);
	}

	char buf[BUFFERSIZE];
	int len, ret, pos;
	int sfd, dfd = 1;
	mytbf_t *tbf;
	tbf = mytbf_init(CPS, BURST);
	if (tbf == NULL)
	{
		fprintf(stderr, "mytbf_init failed!\n");
		exit(1);
	}

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
		int size = mytbf_fetchtoken(tbf, BUFFERSIZE);
		if (size < 0)
		{
			fprintf(stderr, "mytbf_fetchtoken():%s", strerror(-size));
			exit(1);
		}

		while ((len = read(sfd, buf, size)) < 0)
		// 被阻塞了 3s，token = 3；写了3次10个字节
		{
			if (errno == EINTR)
				continue;
			perror("read()");
			exit(1);
		}
		if (len == 0)
			break;

		if (size - len > 0)
			mytbf_returntoken(tbf, size - len);

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
	mytbf_destroy(tbf);
	exit(0);
}