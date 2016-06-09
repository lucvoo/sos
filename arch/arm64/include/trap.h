#ifndef	_ARCH_TRAP_H_
#define	_ARCH_TRAP_H_


static inline void __TRAP(int trap)
{
	asm volatile("brk	%0" :: "n" (trap));
}

static inline void __TRAP_COND(int trap, int cond)
{
	if (cond)
		__TRAP(trap);
}

#endif
