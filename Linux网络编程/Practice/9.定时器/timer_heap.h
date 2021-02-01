#ifndef MIN_HEAP
#define MIN_HEAP

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
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define BUFFER_SIZE 64

class heap_timer;

struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    heap_timer *timer;
};

class heap_timer
{
public:
    heap_timer(int delay)
    {
        expire = time(NULL) + delay;
    }

public:
    time_t expire;
    void (*cb_func)(client_data);
    client_data *user;
    int index;
};

class time_heap
{
public:
    time_heap(int cap) throw();
    time_heap(vector<heap_timer *> init_array) throw();
    ~time_heap();

    void add_timer(heap_timer *timer);
    void del_timer(heap_timer *timer);
    heap_timer* pop_timer();
    void tick();

private:
    void up_adjust(int low, int high);
    void down_adjust(int low, int high);

private:
    vector<heap_timer *> array;
    int valid_size;
};

#endif