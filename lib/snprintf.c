#include "xprintf.h"
#include <printf.h>
#include <string.h>


static int put_buf(const char *str, unsigned n, char *dest, unsigned size)
{
	if (n > size)
		n = size;

	memcpy(dest, str, n);
	return n;
}

void vsnprintf(char *dest, unsigned size, const char *fmt, va_list ap)
{
	unsigned n;

	n = xprintf(put_buf, dest, size - 1, fmt, ap);

	if (n < size)
		dest[n] = 0;
}

void snprintf(char *dest, unsigned size, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(dest, size, fmt, ap);
	va_end(ap);
}
