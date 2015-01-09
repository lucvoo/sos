#include <idle.h>
#include <sched.h>
#include <kapi/kapi.h>
#include <init.h>
#include <compiler.h>


static void __init clear_bss(void)
{
	extern unsigned long __bss_start;
	extern unsigned long __bss_end;
	unsigned long* ptr = &__bss_start;
	unsigned long* end = &__bss_end;

	while (ptr < end) {
		*ptr++ = 0;
	}
}


static void __init initcalls(void)
{
	extern struct initcall __INITCALL_LIST__[];
	struct initcall *p;

	for (p = &__INITCALL_LIST__[0]; p->fun; p++) {
		p->fun();
	}
}


void _os_start(void) __noreturn;

void _os_start(void)
{
	clear_bss();
	initcalls();
	kapi_start();
	_thread_scheduler_start();

	__free_initmem();

	__cpu_idle();
}
