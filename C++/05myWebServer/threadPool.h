#ifndef THREADPOOL
#define THREADPOOL
#include <pthread.h>
#include <iostream>

const int THREADPOOL_INVALID = -1;
const int THREADPOOL_LOCK_FAILURE = -2;
const int THREADPOOL_TASKS_FULL = -3;
const int THREADPOOL_SHUTDOWN = -4;
const int THREADPOOL_THREAD_FAILURE = -5;
const int THREADPOOL_SUCCESS = 0;
const int THREADPOOL_GRACEFUL = 1;

const int MAX_THREAD = 1024;
const int MAX_TASKS = 65535;

struct ThreadPoolTask
{
	void (*func)(void *);
	void *args;
};

class ThreadPool
{
public:
	static int threadpool_create(int thread_nums, int queue_nums);
	static int threadpool_add(void (*fun)(void *), void *arg);
	static void *threadpool_thread(void *arg);

private:
	static pthread_mutex_t tmutex;
	static pthread_cond_t tcond;
	static pthread_t *thread;
	static ThreadPoolTask *tasks;
	static int thread_count;
	static int thread_start_count;
	static int tasks_size;
	static int task_count;
	static int task_head;
	static int task_tail;
	static int shutdown;
};
#endif