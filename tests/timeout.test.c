#include <kapi/kapi.h>
#include <version.h>
#include <utils.h>
#include <timer.h>
#include <sched.h>


static struct thread a __uninit;
static char stack[1024] __stack;

static void fun(void* data)
{
	unsigned long d;

	printf("t = ...\n");

	for (d = 1; ; d += 1) {
		thread_schedule_timeout(d * HZ );
		printf("t = %lx\n", d);
	}
}


void kapi_start(void)
{
	printf(os_version);

	thread_create(&a, 2, fun, 0, stack, sizeof(stack));
	thread_start(&a);
}
