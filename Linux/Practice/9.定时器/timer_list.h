#ifndef LST_TIMER
#define LST_TIMER

#define _GNU_SOURCE 1
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <error.h>
#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#define BUFFER_SIZE 64

class util_timer;

//用户数据结构：客户端socket地址、socket文件描述符、读缓存和定时器
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer *timer;
};

//定时器类
class util_timer
{
public:
    util_timer() : prev(NULL), next(NULL) {}

public:
    time_t expire;                  //任务的超时时间，这里使用绝对时间
    void (*cb_func)(client_data *); //任务回调函数
    client_data *user_data;         //回调函数处理的客户数据，有定时器的执行者传递给回调函数
    util_timer *prev;               //指向前一个定时器
    util_timer *next;               //指向下一个定时器
};

//定时器链表，它是一个升序、双向链表，且带有头结点和尾结点
class sort_timer_lst
{
public:
    sort_timer_lst() : head(NULL), tail(NULL) {}
    ~sort_timer_lst();

    void add_timer(util_timer *timer);
    void adjust_timer(util_timer *timer);
    void del_timer(util_timer *timer);
    void tick();

private:
    void add_timer(util_timer *tiemr, util_timer *lst_head);

private:
    util_timer *head;
    util_timer *tail;
};

#endif