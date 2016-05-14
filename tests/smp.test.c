#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <smp/init.h>
#include <arch/test/fault.h>
#include <sleep.h>
#include <debug.h>
#include <lock.h>


void kapi_start(void)
{
	printf(os_version);

	dump_system_regs();

	__smp_init();

	sleep(1);		// FIXME

	dump_system_regs();
}


void kapi_start_smp(void)
{
	printf("OK?\n");

	dump_system_regs();

        undef_instruction();

	printf("OK!\n");
}
