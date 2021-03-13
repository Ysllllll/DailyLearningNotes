#ifndef THREADPOOL
#define THREADPOOL
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string.h>

const int THREADPOOL_INVALID = -1;
const int THREADPOOL_LOCK_FAILURE = -2;
const int THREADPOOL_COND_FAILURE = -3;
const int THREADPOOL_TASKS_FULL = -4;
const int THREADPOOL_SHUTDOWN = -5;
const int THREADPOOL_THREAD_FAILURE = -6;
const int THREADPOOL_SUCCESS = 0;
const int THREADPOOL_GRACEFUL = 1;

extern std::vector<std::string> errorType; // 注意extern的用法

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
	// 一下函数，成功时均返回0，否则返回对应的error代码
	static int threadpool_create(int _thread_nums, int _queue_nums);
	static int threadpool_add(void (*fun)(void *), void *arg);
	static void *threadpool_thread(void *arg);
	static int threadpool_destroy(int _shutdown);
	static int threadpool_free();

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