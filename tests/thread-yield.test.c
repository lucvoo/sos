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
		printf("thread %s yield ...\n", str);
		thread_yield();
	} while (1);
}


void kapi_start(void)
{
	static char namea[] = "a";
	static char nameb[] = "b";

	printf(os_version);

	thread_create(&a, 2, fun, namea, NULL, 0);
	thread_create(&b, 2, fun, nameb, NULL, 0);
	thread_start(&a);
	thread_start(&b);
}
