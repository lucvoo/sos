#include <string.h>


int memcmp(const void *sa, const void *sb, unsigned int n)
{
	const char *a = sa;
	const char *b = sb;

	while (n--) {
		if (*a++ != *b++)
			return 1;
	}

	return 0;
}
