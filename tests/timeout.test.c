#include <kapi/kapi.h>
#include <version.h>
#include <utils.h>
#include <timer.h>
#include <sched.h>


static struct thread a __uninit;

static void fun(void* data)
{
	unsigned long t, d;

	printf("t = ...\n");

	for (d = 1 << 15, t = d; ; d += 1 << 15, t += d) {
		thread_schedule_timeout(t);
		printf("t = %lx\n", t);
	}
}


void kapi_start(void)
{
	printf(os_version);

	thread_create(&a, 2, fun, 0);
	thread_start(&a);
}
