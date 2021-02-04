#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void printsigset(sigset_t *set)
{
    int i = 0;
    for (; i < sizeof(sigset_t); i++)
        if (sigismember(set, i))
            putchar('1');
        else
            putchar('0');

    printf("\n");
}

int main()
{

    sigset_t s, p;
    sigemptyset(&s);
    sigaddset(&s, SIGINT);
    sigaddset(&s, SIGCHLD);
    sigaddset(&s, SIGTSTP);
    sigaddset(&s, SIGQUIT);
    sigprocmask(SIG_SETMASK, &s, NULL);

    int i = 5;
    while (i--)
    {
        sigpending(&p);
        printsigset(&p);
        sleep(2);
    }

    return 0;
}