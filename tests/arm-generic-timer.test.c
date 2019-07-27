#include <kapi/kapi.h>
#include <arch/generic-timer.h>
#include <utils.h>
#include <timer.h>
#include <sched.h>


static struct thread a __uninit;

static void fun(void* data)
{
	ulong d;

	printf("t = ...\n");

	for (d = 178; ; d += 1) {
		u64 ref, now;
		u32 hi, lo;

		ref = arch_timer_get_counter();
		thread_schedule_timeout(d * HZ);
		now = arch_timer_get_counter();
		now -= ref;
		hi = now >> 32;
		lo = now & 0xffffffff;
		printf("t = %06ld @ (%08lx %08lx)\n\n", d, hi, lo);
	}
}


void kapi_start(void)
{
	thread_create(&a, 2, fun, 0, NULL, 0);
	thread_start(&a);
}
