#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <smp.h>
#include <arch/test/fault.h>
#include <soc/delay.h>
#include <debug.h>
#include <lock.h>


static inline void delay(unsigned long n)
{
	while (n--)
		asm volatile("nop");
}


void kapi_start(void)
{
	printf(os_version);

	dump_system_regs();

	__smp_init();

	delay(__ROUGH_LOOPS_PER_SEC);

	dump_system_regs();
}


void kapi_start_smp(void)
{
	printf("OK?\n");

	dump_system_regs();

        undef_instruction();

	printf("OK!\n");
}
