#ifndef	_PRINTF_H_
#define	_PRINTF_H_

#include <stdarg.h>

void __printf(const char *fmt, ...);
void __vprintf(const char *fmt, va_list ap);
void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list ap);

int snprintf(char *dest, unsigned size, const char *fmt, ...);
uint vsnprintf(char* dest, unsigned size, const char* fmt, va_list ap);

#endif
