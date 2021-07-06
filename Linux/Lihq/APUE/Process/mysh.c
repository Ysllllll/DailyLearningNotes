#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>

#define DELIMS " \t\n"

static void prompt(void)
{
    printf("mysh-0.1$ ");
}

static void parse(char *line, glob_t *globres)
{
    char *tok;
    int i = 0;

    while (1)
    {
        tok = strsep(&line, DELIMS);
        if (tok == NULL)
        {
            break;
        }
        if (tok[0] == '\0') //多个分隔符连续
            continue;
        glob(tok, GLOB_NOCHECK | (GLOB_APPEND * i), NULL, globres);
        i = 1;
    }
    glob(NULL, GLOB_NOCHECK | (GLOB_APPEND * i), NULL, globres); // 注意了，没有这一句execvp不能执行
}

int main()
{
    char *linebuf = NULL; //必须初始化
    size_t linebuf_size = 0;
    glob_t globres;
    pid_t pid;

    while (1)
    {
        prompt();

        if (getline(&linebuf, &linebuf_size, stdin) < 0)
            break;

        parse(linebuf, &globres);

        if (0) //是内部命令
        {
        }
        else // 是外部命令
        {
            pid = fork();
            fflush(NULL);
            if (pid < 0)
            {
                perror("fork():");
                exit(1);
            }
            if (pid == 0) //child
            {
                fflush(NULL);
                /*
                    //可执行，有输出
                    char *argv[] = {"ls", NULL};
                    execvp(argv[0], argv);
                    
                    //可执行，无输出
                    char *argv[] = {"ls"};
                    execvp(argv[0], argv);
                */
                execvp(globres.gl_pathv[0], globres.gl_pathv); // execvp要求 gl_pathv 以 NULL 结尾
                perror("execvp()");
                exit(1);
            }
            else //parent
            {
                waitpid(pid, NULL, 0);
            }
        }
    }

    exit(0);
}