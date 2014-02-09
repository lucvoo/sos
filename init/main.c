#include "init-internal.h"
#include <idle.h>
#include <sched.h>
#include <kapi/kapi.h>
#include <os.h>
#include <init.h>

void platform_init(void);	// FIXME
void timer_init(void);		// FIXME

void _os_start(void)
{
	__clear_bss();
	_os_initcalls();
	platform_init();	// FIXME: arch_initcall
	timer_init();		// FIXME: subsys_initcall
	kapi_start();
	_thread_scheduler_start();

	__free_initmem();

	__cpu_idle();
}
