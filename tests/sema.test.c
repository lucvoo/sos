#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>
#include <semaphore.h>
#include <timer.h>
#include "tests-utils.h"


static struct semaphore sema;

static struct thread a __uninit;
static struct thread b __uninit;
static struct thread c __uninit;

static void fun(void* data)
{
	const char* str = data;

	do {
		unsigned long delta;

		printf("thread %s try semaphore\n", str);
		semaphore_wait(&sema);
		printf("thread %s got semaphore\n", str);
		thread_yield();
		printf("thread %s rel semaphore\n", str);
		semaphore_post(&sema);
		printf("thread %s pos semaphore\n", str);
#if 0
		delta = ((sprng() | 1) & 0x7)  << 5;
		thread_schedule_timeout(delta);
#endif
	} while (1);
}

void kapi_start(void)
{
	static char namea[] = "a";
	static char nameb[] = "b";
	static char namec[] = "c";

	printf(os_version);

	semaphore_init(&sema, 1);

	thread_create(&a, 2, fun, namea, NULL, 0);
	thread_create(&b, 2, fun, nameb, NULL, 0);
	thread_start(&a);
	thread_start(&b);

	thread_create(&c, 2, fun, namec, NULL, 0);
	thread_start(&c);
}
