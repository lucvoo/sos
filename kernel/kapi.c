#include <kapi.h>

#include <thread.h>
#include <idle.h>

void kapi_scheduler_start(void)
{
	__cpu_idle();
}
