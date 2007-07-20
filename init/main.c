#include <idle.h>
#include <sched.h>
#include <kapi.h>

void __clear_bss(void);
void _os_initcalls(void);

void _os_start(void)
{
	__clear_bss();
	_os_initcalls();
	kapi_start();
	_thread_scheduler_start();

	// remove .init sections

	__cpu_idle();
}
