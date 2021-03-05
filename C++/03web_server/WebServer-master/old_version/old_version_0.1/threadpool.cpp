#include "threadpool.h"

threadpool_t *threadpool_create(int thread_count, int queue_size, int flags)
{
	threadpool_t *pool;
	int i;
	//(void) flags;
	do
	{
		if (thread_count <= 0 || thread_count > MAX_THREADS || queue_size <= 0 || queue_size > MAX_QUEUE)
		{
			return NULL;
		}

		if ((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL)
		{
			break;
		}

		/* Initialize */
		pool->thread_count = 0; //这里为何不初始化为0？
		pool->queue_size = queue_size;
		pool->head = pool->tail = pool->count = 0;
		pool->shutdown = pool->started = 0;

		/* Allocate thread and task queue */
		pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
		pool->queue = (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_size);

		/* Initialize mutex and conditional variable first */
		if ((pthread_mutex_init(&(pool->lock), NULL) != 0) ||
			(pthread_cond_init(&(pool->notify), NULL) != 0) ||
			(pool->threads == NULL) ||
			(pool->queue == NULL))
		{
			break;
		}
		memset(pool->threads, 0, sizeof(pthread_t) * thread_count);
		memset(pool->queue, 0, sizeof(threadpool_task_t) * queue_size);

		/* Start worker threads */
		for (i = 0; i < thread_count; i++)
		{
			//创建线程就是创建一个执行流，即一个函数调用，只是这个函数在执行时可以不执行完就去执行其它函数
			//注意threadpool_thread这个函数的返回值不会在这里被接收，这里只是判断这个线程有没有创建成功，而不是判断这个线程的返回值怎么样
			if (pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void *)pool) != 0)
			{
				//没有创建成功就销毁
				threadpool_destroy(pool, 0);
				return NULL;
			}
			pool->thread_count++;
			pool->started++;
		}

		return pool;
	} while (false);

	// 下面的执行流只有在 pool 创建成功，线程创建前出现了问题，执行pool已分配资源的回收。
	if (pool != NULL)
	{
		threadpool_free(pool);
	}
	return NULL;
}

int threadpool_add(threadpool_t *pool, void (*function)(void *), void *argument, int flags)
{
	//printf("add to thread pool !\n");
	int err = 0;
	int next;
	//(void) flags;
	if (pool == NULL || function == NULL)
	{
		return THREADPOOL_INVALID;
	}
	if (pthread_mutex_lock(&(pool->lock)) != 0)
	{
		return THREADPOOL_LOCK_FAILURE;
	}
	next = (pool->tail + 1) % pool->queue_size;
	do
	{
		/* Are we full ? */
		if (pool->count == pool->queue_size)
		{
			err = THREADPOOL_QUEUE_FULL;
			break;
		}
		/* Are we shutting down ? */
		if (pool->shutdown)
		{
			err = THREADPOOL_SHUTDOWN;
			break;
		}
		/* Add task to queue */
		pool->queue[pool->tail].function = function;
		pool->queue[pool->tail].argument = argument;
		pool->tail = next;
		pool->count += 1;

		/* pthread_cond_broadcast */
		if (pthread_cond_signal(&(pool->notify)) != 0)
		{
			err = THREADPOOL_LOCK_FAILURE;
			break;
		}
	} while (false);

	if (pthread_mutex_unlock(&pool->lock) != 0)
	{
		err = THREADPOOL_LOCK_FAILURE;
	}

	return err;
}

int threadpool_destroy(threadpool_t *pool, int flags)
{
	printf("Thread pool destroy !\n");
	int i, err = 0;

	if (pool == NULL)
	{
		return THREADPOOL_INVALID;
	}

	if (pthread_mutex_lock(&(pool->lock)) != 0)
	{
		return THREADPOOL_LOCK_FAILURE;
	}

	do
	{
		/* Already shutting down */
		if (pool->shutdown)
		{
			err = THREADPOOL_SHUTDOWN;
			break;
		}

		// 可以看出一个线程创建失败就关闭所有线程，当然如果有线程还有工作则先要等到执行完之后再关闭
		pool->shutdown = (flags & THREADPOOL_GRACEFUL) ? graceful_shutdown : immediate_shutdown;

		/* Wake up all worker threads */
		if ((pthread_cond_broadcast(&(pool->notify)) != 0) ||
			(pthread_mutex_unlock(&(pool->lock)) != 0))
		{
			err = THREADPOOL_LOCK_FAILURE;
			break;
		}

		/* Join all worker thread */
		for (i = 0; i < pool->thread_count; ++i)
		{
			if (pthread_join(pool->threads[i], NULL) != 0)
			{
				err = THREADPOOL_THREAD_FAILURE;
			}
		}
	} while (false);

	/* Only if everything went well do we deallocate the pool */
	if (!err)
	{
		threadpool_free(pool);
	}
	return err;
}

int threadpool_free(threadpool_t *pool)
{
	if (pool == NULL || pool->started > 0)
	{
		return -1;
	}

	/* Did we manage to allocate ? */
	if (pool->threads)
	{
		free(pool->threads);
		free(pool->queue);

		/* Because we allocate pool->threads after initializing the
           mutex and condition variable, we're sure they're
           initialized. Let's lock the mutex just in case. */
		pthread_mutex_lock(&(pool->lock));
		pthread_mutex_destroy(&(pool->lock));
		pthread_cond_destroy(&(pool->notify));
	}
	free(pool);
	return 0;
}

static void *threadpool_thread(void *threadpool)
{
	threadpool_t *pool = (threadpool_t *)threadpool;
	threadpool_task_t task;

	for (;;) // 无限循环，当没有任务的时候退出，此时也表明这个线程可以退休了
	{
		/* Lock must be taken to wait on conditional variable */
		pthread_mutex_lock(&(pool->lock));

		/* Wait on condition variable, check for spurious wakeups.
           When returning from pthread_cond_wait(), we own the lock. */
		while ((pool->count == 0) && (!pool->shutdown))
		{
			pthread_cond_wait(&(pool->notify), &(pool->lock));
		}

		if ((pool->shutdown == immediate_shutdown) ||
			((pool->shutdown == graceful_shutdown) &&
			 (pool->count == 0)))
		{
			break;
		}

		/* Grab our task */
		// 注意 线程（thread） 与 任务（task） 没有任何对应关系
		task.function = pool->queue[pool->head].function;
		task.argument = pool->queue[pool->head].argument;
		pool->head = (pool->head + 1) % pool->queue_size;
		pool->count -= 1;

		/* Unlock */
		pthread_mutex_unlock(&(pool->lock));

		/* Get to work */
		(*(task.function))(task.argument);
	}

	//正常情况下它不会退出
	--pool->started;

	pthread_mutex_unlock(&(pool->lock));
	pthread_exit(NULL);
	return (NULL);
}