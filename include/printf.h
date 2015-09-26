#ifndef	_PRINTF_H_
#define	_PRINTF_H_

#include <stdarg.h>

void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list ap);

void snprintf(char *dest, unsigned size, const char *fmt, ...);
uint vsnprintf(char* dest, unsigned size, const char* fmt, va_list ap);

#endif
