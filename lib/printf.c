#include "xprintf.h"
#include <printf.h>
#include <stdarg.h>

int putx(const char *buf, unsigned n);
static int _putx(const char *buf, unsigned n, char *dest, unsigned size)
{
	(void)dest;
	(void)size;
	putx(buf, n);

	return n;
}

void vprintf(const char *fmt, va_list ap)
{
	xprintf(_putx, 0, -1, fmt, ap);
}

void printf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	xprintf(_putx, 0, -1, fmt, ap);
	va_end(ap);
}
