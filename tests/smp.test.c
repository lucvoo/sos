#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <smp/init.h>
#include <arch/test/fault.h>
#include <sleep.h>
#include <debug.h>
#include <lock.h>
#include <smp.h>


static struct thread main __uninit;

static void start_smp(void *data)
{
	dump_system_regs();

	__smp_init();

	sleep(3);		// FIXME
	printf("main OK!\n");
}

void kapi_start(void)
{
	printf(os_version);

	thread_create(&main, 2, start_smp, NULL, NULL, 0);
	thread_start(&main);
}


// This is the CPU's idle thread. It must not sleep!
void kapi_start_smp(void)
{
	uint cpu = __cpuid();

	printf("cpu %d: OK?\n", cpu);

	dump_system_regs();

	printf("cpu %d: test undef exception (should dump & halt)\n", cpu);
        undef_instruction();

	printf("cpu %d: still running? Doh!\n", cpu);
}
