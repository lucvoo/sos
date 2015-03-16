#include <version.h>
#include <thread.h>

static struct thread a, b;

static void fun(void* data)
{
	const char* str = data;

	do {
		printf("thread %s running\n", str);
		thread_yield();
	} while (1);
}

void kapi_start(void)
{
	printf(os_version);

	thread_create(&a, 2, fun, "a");
	thread_create(&b, 2, fun, "b");
	thread_start(&a);
	thread_start(&b);
}
