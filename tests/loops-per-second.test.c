#include <kapi/kapi.h>
#include <version.h>
#include <utils.h>
#include <timer.h>
#include <sched.h>
#include <timer.h>


static struct thread a __uninit;
static struct timer timer;
static volatile int counter;
static volatile int lpj;


static void expire(void *data)
{
	lpj = -counter;

	counter = 1;
}

static void loops_per_second(void* data)
{
	timer.action = expire;
	timer.data = &timer;

	counter = 0;
	printf("lpj = %d\n", lpj);
	timer_add_rel(&timer, HZ);
	while (--counter) {
	}

	printf("lpj = %d\n", lpj);
}


void kapi_start(void)
{
	printf(os_version);

	thread_create(&a, 2, loops_per_second, 0, 0, 0);
	thread_start(&a);
}
