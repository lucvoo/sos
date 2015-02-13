#ifndef	_DEBUG_H_
#define	_DEBUG_H_

#include <arch/debug.h>


#ifdef	DEBUG
#define	dbg(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
static inline void dbg(const char *fmt, ...)
{
	(void) fmt;
}
#endif

void dump_system_regs(void);

#endif
