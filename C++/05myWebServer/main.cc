#include "threadPool.h"

int main()
{
	ThreadPool::threadpool_create(4, 1024);
	return 0;
}