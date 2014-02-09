#include "init-internal.h"
#include <idle.h>
#include <sched.h>
#include <kapi/kapi.h>
#include <os.h>
#include <init.h>


void _os_start(void)
{
	__clear_bss();
	_os_initcalls();
	kapi_start();
	_thread_scheduler_start();

	__free_initmem();

	__cpu_idle();
}
