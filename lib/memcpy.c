#include <string.h>
#include <symbols.h>


static void memcpy_generic(void *dst, const void *src, unsigned int n)
{
	const char *s = src;
	char *d = dst;

	while (n--)
		*d++ = *s++;
}
weak_alias(memcpy_generic, memcpy);
