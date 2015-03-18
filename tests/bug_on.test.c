#include <kapi/kapi.h>
#include <version.h>
#include <bug.h>


static void __noinline test_bugon(int cond)
{
	BUG_ON(cond);
}

void kapi_start(void)
{
	printf(os_version);

	BUG_ON(0);
	printf("OK\n");

	test_bugon(0);
	printf("OK\n");

	test_bugon(1);
	printf("Humpf!\n");
}
