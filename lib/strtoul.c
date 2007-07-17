#include <string.h>


int strtoul(const char* str, unsigned int* val)
{
	const char* s = str;
	unsigned int i = 0;
	unsigned int c;

	while ((c = *s++ - '0') <= 9) {
		i = i * 10 + c;
	}

	*val = i;
	return (s-1) - str;
}
