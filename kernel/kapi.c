#include <kapi.h>

#include <sched.h>
#include <idle.h>

void kapi_scheduler_start(void)
{
	_thread_scheduler_start();
	__cpu_idle();
}
