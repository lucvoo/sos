#include <idle.h>
#include <sched.h>
#include <kapi.h>

void __clear_bss(void);
void _os_initcalls(void);
void platform_init(void);

void _os_start(void)
{
	__clear_bss();
	_os_initcalls();
	platform_init();	// FIXME: arch_initcall
	kapi_start();
	_thread_scheduler_start();

	// remove .init sections

	__cpu_idle();
}
