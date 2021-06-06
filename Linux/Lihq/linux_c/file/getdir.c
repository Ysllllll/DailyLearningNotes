#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

// 读取 etc 文件夹下的所有文件
#define PAT "/bin/"

int main()
{
    DIR *dp = opendir(PAT);
    struct dirent *cur;
    while ((cur = readdir(dp)) != NULL)
        puts(cur->d_name);

    closedir(dp);

    exit(0);
}