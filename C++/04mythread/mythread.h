#include <ucontext.h>

typedef void (*Fun)(void *arg);

typedef struct mythread_t
{
	ucontext_t ctx;
	Fun func;
	void *arg;
	enum ThreadState state;
	char stack[DEFAULT_STACK_SIZE];
};