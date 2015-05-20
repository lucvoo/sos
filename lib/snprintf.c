#include "xprintf.h"
#include <printf.h>
#include <string.h>


static void snprintf_put(const char *str, unsigned int n, struct xput *xput)
{
	unsigned int size = xput->size;

	if (n > size)
		n = size;

	memcpy(xput->dest, str, n);

	xput->dest += n;
	xput->size -= n;
}


void vsnprintf(char *dest, unsigned size, const char *fmt, va_list ap)
{
	struct xput xput = {
		.func = snprintf_put,
		.dest = dest,
		.size = size-1,
	};
	unsigned n;

	n = xprintf(&xput, fmt, ap);
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
