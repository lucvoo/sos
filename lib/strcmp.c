#include <string.h>


int strcmp(const char *sa, const char *sb)
{
	unsigned int ca, cb;

	while ((ca = *sa++) == (cb = *sb++)) {
		if (!ca)
			return 0;
	}

	return (ca < cb) ? -1 : 1;
}
