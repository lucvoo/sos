#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <smp/init.h>
#include <smp.h>
#include <lock.h>


static struct lock lock;


static void do_loop(void)
{
	uint cpu = __cpuid();
	int n = 0;

	printf("cpu%d started\n", cpu);

	lock_acq(&lock);
	printf("\n");
	lock_rel(&lock);

	while (1) {
		lock_acq(&lock);
		__printf("thr%d: %d\n", cpu, n++);
		lock_rel(&lock);
	}
}

void kapi_start(void)
{
	printf(os_version);

	lock_acq(&lock);
	__smp_init();
	lock_rel(&lock);

	do_loop();
}


void kapi_start_smp(void)
{
	do_loop();
}
