#include <kapi/kapi.h>
#include <arch/generic-timer.h>
#include <utils.h>
#include <timer.h>
#include <sched.h>


#define	SECS	10

static struct thread a __uninit;

static void fun(void* data)
{
	printf("t = ...\n");

	while (1) {
		u64 t0, t1;
		ulong d;

		t0 = arch_timer_get_counter();
		thread_schedule_timeout(SECS * HZ);
		t1 = arch_timer_get_counter();
		d = t1 - t0;
		printf("d = %06ld\n", d);
	}
}


void kapi_start(void)
{
	thread_create(&a, 2, fun, 0, NULL, 0);
	thread_start(&a);
}
