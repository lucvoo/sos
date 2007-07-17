#include <string.h>


unsigned int strlen(const char *str)
{
	const char *s = str;
	unsigned int c;

	while ((c = *s++))
		;

	return s - str - 1;
}
