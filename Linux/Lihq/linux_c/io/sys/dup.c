#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FNAME "/tmp/out"
int main()
{

	int fd;
	//close(1);

	// 情况一：fd = 1。（自证）
	// 情况二：close时，有其它程序立即占用了 1，此时dup到的最小fd将不再是 1。（原子性问题）
	fd = open(FNAME, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("open()");
		exit(1);
	}
	// close(1);
	// dup(fd);
	dup2(fd, 1); // 上面两句话的原子操作。关闭 1，然后将 fd 复制到 1。同时解决了上面可能出现的两种情况。

	if (fd != 1)
		close(fd);

	puts("hello!");

	exit(0);
}
