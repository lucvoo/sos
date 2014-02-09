#include <string.h>
#include <stddef.h>


void* memchr(const void *src, int c, unsigned int n)
{
	const unsigned char* s = src;
	unsigned int i;

	for (i=0; i < n; i++) {
		if (s[i] == c)
			return (void*) (unsigned long) &s[i];
	}

	return NULL;
}
