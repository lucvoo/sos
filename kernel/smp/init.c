#include <kapi/kapi.h>
#include <smp/init.h>
#include <smp/ops.h>
#include <smp.h>
#include <symbols.h>
#include <idle.h>
#include <sched.h>


void __smp_start(void)
{
	uint cpu = __coreid();

	smp_ops.init_cpu(cpu);

	kapi_start_smp();
	_thread_scheduler_start();
	__cpu_idle();
}


void __weak kapi_start_smp(void)
{
	// Do nothing
}
