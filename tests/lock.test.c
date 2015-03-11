#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <smp/init.h>
#include <lock.h>


static struct lock lock;

void kapi_start(void)
{
	int n = 0;

	printf(os_version);

	lock_acq(&lock);
	__smp_init();
	lock_rel(&lock);

	while (1) {
		lock_acq(&lock);
		++n;
		printf("CORE 0: %08x\n", n);
		lock_rel(&lock);
	}
}


void kapi_start_smp(void)
{
	printf("SMP started\n");

	while (1) {
		lock_acq(&lock);
		printf("CORE 1\n");
		lock_rel(&lock);
	}
}
