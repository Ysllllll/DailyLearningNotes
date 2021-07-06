#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

    char *argv[] = {"ls"};
    execvp(argv[0], argv);

    // char *args[] = {};
    // args[0] = "ls";
    // args[1] = "-al";
    // pid_t pid = fork();
    // if (pid == 0)
    // {
    //     execvp(args[0], args); //执行ls -al
    //     exit(1);
    // }
    // else
    // {
    //     wait(NULL);
    // }

    exit(0);
}