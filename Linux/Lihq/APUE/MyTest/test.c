#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern char **environ;

int main(int argc, char **argv)
{
	int i;
	puts(getenv("PATH"));
	exit(0);
}