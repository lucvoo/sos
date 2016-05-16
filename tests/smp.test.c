#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <smp/init.h>
#include <arch/test/fault.h>
#include <sleep.h>
#include <debug.h>
#include <lock.h>


static struct thread main __uninit;

static void start_smp(void *data)
{
	dump_system_regs();

	__smp_init();

	sleep(3);		// FIXME
	printf("main OK!\n");
	dump_system_regs();
}

void kapi_start(void)
{
	printf(os_version);

	thread_create(&main, 2, start_smp, NULL, NULL, 0);
	thread_start(&main);
}


void kapi_start_smp(void)
{
	printf("OK?\n");

	dump_system_regs();

        undef_instruction();

	printf("OK!\n");
}
