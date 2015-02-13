#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <debug.h>


void kapi_start(void)
{
	printf(os_version);

	dump_system_regs();
}
