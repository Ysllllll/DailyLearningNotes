#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *func(void *s)
{
    while (1)
        pause();
    pthread_exit(NULL);
}
int main()
{

    int i = 0;
    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1024 * 1024);

    for (i = 0;; i++)
    {
        int err = pthread_create(&tid, &attr, func, NULL);
        if (err)
        {
            fprintf(stderr, "pthread_create():%s\n", strerror(err));
            break;
        }
    }
    printf("i = %d\n", i);
    pthread_attr_destroy(&attr);
    exit(0);
}