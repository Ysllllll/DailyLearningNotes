#include "threadPool.h"

pthread_mutex_t ThreadPool::tmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::tcond = PTHREAD_COND_INITIALIZER;
pthread_t *ThreadPool::thread = NULL;
ThreadPoolTask *ThreadPool::tasks = NULL;
int ThreadPool::thread_count = 0;
int ThreadPool::tasks_size = 0;
int ThreadPool::task_head = 0;
int ThreadPool::task_tail = 0;
int ThreadPool::task_count = 0;
int ThreadPool::thread_start_count = 0;
int ThreadPool::shutdown = 0;

int ThreadPool::threadpool_create(int _thread_count, int _tasks_size)
{
	if (_thread_count <= 0 || _thread_count > MAX_THREAD || _tasks_size <= 0 || _tasks_size > MAX_TASKS)
	{
		_thread_count = 4;
		_tasks_size = 1024;
	}
	thread = new pthread_t[_thread_count];
	tasks = new ThreadPoolTask[_tasks_size];
	if (pthread_mutex_init(&tmutex, NULL) != 0 || pthread_cond_init(&tcond, NULL) != 0 || !thread || !tasks)
		return THREADPOOL_LOCK_FAILURE;

	for (int i = 0; i < _thread_count; i++)
	{
		if (pthread_create(&thread[i], NULL, threadpool_thread, NULL) != 0)
		{
			return THREADPOOL_THREAD_FAILURE;
		}
		thread_count++;
		thread_start_count++;
		std::cout << "create thread : " << thread[i] % 100 << std::endl;
	}
	return THREADPOOL_SUCCESS;
}

int ThreadPool::threadpool_add(void (*fun)(void *), void *arg)
{
	if (pthread_mutex_lock(&tmutex) != 0)
		return THREADPOOL_LOCK_FAILURE;
	if (task_count == tasks_size)
		return THREADPOOL_TASKS_FULL;
	if (shutdown)
		return THREADPOOL_SHUTDOWN;

	tasks[task_tail].func = fun;
	tasks[task_tail].args = arg;
	task_tail = (task_tail + 1) % tasks_size;
	task_count++;

	if (pthread_cond_signal(&tcond) != 0)
		return THREADPOOL_LOCK_FAILURE;

	if (pthread_mutex_unlock(&tmutex) != 0)
		return THREADPOOL_LOCK_FAILURE;
}

void *ThreadPool::threadpool_thread(void *arg)
{
	ThreadPoolTask task;

	while (true)
	{
		pthread_mutex_lock(&tmutex);
		while (task_count == 0 || !shutdown)
		{
			pthread_cond_wait(&tcond, &tmutex);
		}
		if (shutdown || task_count == 0)
		{
			break;
		}

		task.func = tasks[task_head].func;
		task.args = tasks[task_head].args;
		tasks[task_head].func = NULL;
		tasks[task_head].args = NULL;

		task_head = (task_head + 1) % MAX_TASKS;
		task_count--;

		pthread_mutex_unlock(&tmutex);

		(task.func)(task.args);
	}
	std::cout << "thread quit" << std::endl;
	thread_start_count--;
	pthread_mutex_unlock(&tmutex);
	pthread_exit(NULL);
}