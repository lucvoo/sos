#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>


extern void dump_system_regs(void);
void kapi_start(void)
{
	printf(os_version);

	dump_system_regs();
}
