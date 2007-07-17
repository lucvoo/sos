#include <string.h>
#include <symbols.h>


static void memset_generic(void *dst, int c, unsigned int n)
{
	const char *end = dst + n;
	char *d = dst;

	while (d < end)
		*d++ = c;
}
weak_alias(memset_generic, memset);
