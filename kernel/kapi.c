#include <kapi.h>

#include <thread.h>

void kapi_scheduler_start(void)
{
	thread_schedule();
}
