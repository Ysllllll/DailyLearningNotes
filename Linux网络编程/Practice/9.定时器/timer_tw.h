#ifndef TIME_WHEEL_TIMER
#define TIME_WHEEL_TIMER
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

class tw_timer;

struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    tw_timer *timer;
};

class tw_timer
{
public:
    tw_timer(int rot, int ts) : next(NULL), prev(NULL), rotation(rot), time_slot(ts) {}

public:
    int rotation;
    int time_slot;
    void (*cb_func)(client_data *);
    client_data *user_data;
    tw_timer *next;
    tw_timer *prev;
};

class time_wheel
{
public:
    time_wheel();
    ~time_wheel();
    tw_timer *add_timer(int timeout);
    void del_timer(tw_timer *timer);
    void tick();
    static int get_tick_SI();

private:
    static const int N = 60;
    static const int SI = 1;
    tw_timer *slots[N];
    int cur_slot;
};

#endif