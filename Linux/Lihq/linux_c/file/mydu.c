#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>
#include <string.h>
#include <stdbool.h>

#define PATHSIZE 1024

static bool path_noloop(const char *path)
{
    char *pos = strrchr(path, '/');
    if (pos == NULL)
    {
        exit(1);
    }

    if (strcmp(pos + 1, ".") == 0 || strcmp(pos + 1, "..") == 0)
        return false;
    else
        return true;
}

static int64_t mydu(const char *path)
{
    struct stat statres;
    glob_t globres;
    char nextpath[PATHSIZE];
    int64_t sum;

    if (lstat(path, &statres) < 0)
    {
        perror("lsat():");
        exit(1);
    }

    //判断是目录还是非目录，两种形式：宏、位图
    if (!S_ISDIR(statres.st_mode))
        return statres.st_blocks;

    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/*", PATHSIZE);
    glob(nextpath, 0, NULL, &globres);

    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/.*", PATHSIZE);
    glob(nextpath, GLOB_APPEND, NULL, &globres);

    for (int i = 0; i < globres.gl_pathc; i++)
        if (path_noloop(globres.gl_pathv[i]))
            sum += mydu(globres.gl_pathv[i]);

    sum += statres.st_blocks;
    return sum;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    printf("%ld\n", mydu(argv[1]) / 2);

    exit(0);
}