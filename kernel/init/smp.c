#include <kapi/kapi.h>
#include <symbols.h>
#include <idle.h>


void __smp_start(void) __noreturn;	// Called from low-level SMP startup code
void __smp_start(void)
{
	kapi_start_smp();
	__cpu_idle();
}


void __weak kapi_start_smp(void)
{
	// Do nothing
}
