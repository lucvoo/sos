#ifndef	_GENERIC_BUG_H_
#define	_GENERIC_BUG_H_

#include <trap.h>


static inline void __BUG_ON(const char *file, const char *func, int line, int cond)
{
	__TRAP_COND(TRAP_BUG, cond);
}

static inline void __BUG(const char *file, const char *func, int line)
{
	__TRAP(TRAP_BUG);
}

#endif
