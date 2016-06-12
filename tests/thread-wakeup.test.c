#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>

static struct thread a __uninit;
static struct thread b __uninit;

static void fun(void* data)
{
	const char* str = data;

	do {
		printf("thread %s going to sleep ...\n", str);
		thread_sleep();
		printf("thread %s running\n", str);
		thread_yield();
	} while (1);
}

static struct thread c __uninit;

static void func(void* data)
{
	const char* str = data;

	do {
		printf("thread %s wakeup a\n", str);
		thread_wakeup(&a);
		thread_yield();
		printf("thread %s wakeup b\n", str);
		thread_wakeup(&b);
		thread_yield();
	} while (1);
}

void kapi_start(void)
{
	static char namea[] = "a";
	static char nameb[] = "b";
	static char namec[] = "c";

	printf(os_version);

	thread_create(&a, 2, fun, namea, NULL, 0);
	thread_create(&b, 2, fun, nameb, NULL, 0);
	thread_start(&a);
	thread_start(&b);

	thread_create(&c, 2, func, namec, NULL, 0);
	thread_start(&c);
}
