#ifndef	_XPRINTF_H_
#define	_XPRINTF_H_

#include <stdarg.h>

typedef int (*put_fn_t)(const char* str, unsigned int n, char* dest, unsigned size);
unsigned xprintf(put_fn_t put, char* dest, unsigned size, const char* fmt, va_list ap);

#endif
