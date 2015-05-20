#include "xprintf.h"
#include <printf.h>
#include <stdarg.h>
#include <lock.h>

struct lock printf_lock;

int putx(const char *buf, unsigned n);	// FIXME


static struct xput printf_xput = {
	.func = (void *) putx,		// hack for fast path
	.dest = NULL,
	.size = 0,
};

void vprintf(const char *fmt, va_list ap)
{
	xprintf(&printf_xput, fmt, ap);
}

void printf(const char *fmt, ...)
{
	unsigned long flags;
	va_list ap;

	va_start(ap, fmt);
	flags = lock_acq_save(&printf_lock);
	xprintf(&printf_xput, fmt, ap);
	lock_rel_rest(&printf_lock, flags);
	va_end(ap);
}
