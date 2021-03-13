#include "threadPool.h"

void func1(void *arg)
{
	std::cout << "func1" << std::endl;
	sleep(3);
	std::cout << "func1 sleep over" << std::endl;
}
void func2(void *arg)
{
	std::cout << "func2" << std::endl;
	sleep(3);
	std::cout << "func2 sleep over" << std::endl;
}
void func3(void *arg)
{
	std::cout << "func3" << std::endl;
	sleep(3);
	std::cout << "func3 sleep over" << std::endl;
}
int main()
{
	std::cout << ThreadPool::threadpool_create(3, 1024) << std::endl;
	ThreadPool::threadpool_add(func1, NULL);
	ThreadPool::threadpool_add(func2, NULL);
	ThreadPool::threadpool_add(func3, NULL);

	sleep(10);
	int shutdown = 1;
	ThreadPool::threadpool_destroy(shutdown);
	return 0;
}