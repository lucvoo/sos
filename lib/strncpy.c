#include <string.h>


// FIXME: use a 'safe' version !!!
void strncpy(char *dst, const char *src, unsigned int max)
{
	char *end = dst + max;

	while (dst < end) {
		if ((*dst++ = *src++) == 0)
			return;
	}

	// FIXME
}
