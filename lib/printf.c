#include "xprintf.h"
#include <printf.h>
#include <stdarg.h>
#include <lock.h>

struct lock printf_lock;

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
	xprintf(_putx, NULL, -1, fmt, ap);
}

void printf(const char *fmt, ...)
{
	unsigned long flags;
	va_list ap;

	va_start(ap, fmt);
	flags = lock_acq_save(&printf_lock);
	xprintf(_putx, NULL, -1, fmt, ap);
	lock_rel_rest(&printf_lock, flags);
	va_end(ap);
}
