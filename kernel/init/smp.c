#include <kapi/kapi.h>
#include <smp/init.h>
#include <symbols.h>
#include <idle.h>
#include <sched.h>


void __smp_start(void)
{
	kapi_start_smp();
	_thread_scheduler_start();
	__cpu_idle();
}


void __weak kapi_start_smp(void)
{
	// Do nothing
}
