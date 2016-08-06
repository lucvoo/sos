#include <idle.h>
#include <sched.h>
#include <kapi/kapi.h>
#include <init.h>
#include <compiler.h>
#include <symbols.h>


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

static void __init init_data(void)
{
	extern const unsigned long __data_load;
	extern const unsigned long __data_end;
	extern unsigned long __data_start;

	if (kconfig(XIP)) {
		const unsigned long *src = &__data_load;
		const unsigned long *end = &__data_end;
		unsigned long *dst = &__data_start;

		while (dst < end)
			*dst++ = *src++;
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


void _os_start(void) __noreturn __weak;

void _os_start(void)
{
	clear_bss();
	init_data();
	initcalls();
	__free_initmem();

	__sched_start(0);
	kapi_start();
	__cpu_idle();
}
