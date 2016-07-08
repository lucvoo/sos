#ifndef	_TRAP_H_
#define	_TRAP_H_

#include <arch/trap.h>

#ifndef	__TRAP_COND
#define	__TRAP_COND(TRAP, COND)		do { if (COND) __TRAP(TRAP); } while (0)
#endif

#define	TRAP_UNUSED	0
#define	TRAP_TEST	1
#define	TRAP_BUG	2
#define	TRAP_DIV0_MIPS	7

#endif
