#include <iostream>
#include <ucontext.h>
#include <unistd.h>

void func1(void *arg)
{
	puts("1");
	puts("11");
	puts("111");
	puts("1111");
}

void context_test()
{
	char stack[1024 * 128];
	ucontext_t child, main;
	getcontext(&child); //尝试删除这一句看看程序会发生啥，程序会崩溃，因为child没有初始化，尽管我们在上一句已经定义了这个对象

	child.uc_stack.ss_sp = stack; //需要一个实体来初始化，尽管child = 现在的context
	child.uc_stack.ss_size = sizeof(stack);
	child.uc_stack.ss_flags = 0;
	child.uc_link = &main;

	makecontext(&child, (void (*)(void))func1, 0);

	swapcontext(&main, &child); //main中保存的是child原有的东西，但是它不是从child中拿过来，而是从系统中获取，等同于getcontext(&main)
	puts("main");
}
int main()
{
	context_test();

	return 0;
}