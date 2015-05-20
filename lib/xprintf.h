#ifndef	_XPRINTF_H_
#define	_XPRINTF_H_

#include <stdarg.h>

struct xput {
	void (*func)(const char* str, unsigned int n, struct xput *xput);
	char *dest;
	unsigned int size;
};

unsigned int xprintf(struct xput *xput, const char *fmt, va_list ap);

#endif
