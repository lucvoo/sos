#include <version.h>
#include <arch/test/fault.h>


void kapi_start(void)
{
	printf(os_version);

	undef_instruction();

	printf("OK!\n");
}
