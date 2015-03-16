#include <kapi/kapi.h>
#include <version.h>
#include <bug.h>


void kapi_start(void)
{
	printf(os_version);

	BUG();

	printf("Humpf!\n");
}
