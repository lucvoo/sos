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
		u32 s, f, fns;

		ref = arch_timer_get_counter();
		thread_schedule_timeout(d * HZ);
		now = arch_timer_get_counter();
		now -= ref;
		hi = now >> 32;
		lo = now & 0xffffffff;
		s = now / HZ;
		f = (now % HZ);
		fns = ((now % HZ) * 1000000000)/HZ;

		printf("t = %06ld @ (%08lx %08lx) %06ld.%09ld (%08lx)\n\n", d, hi, lo, s, fns, f);
	}
}


void kapi_start(void)
{
	thread_create(&a, 2, fun, 0, NULL, 0);
	thread_start(&a);
}
