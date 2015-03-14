#ifndef	_ARCH_TRAP_H_
#define	_ARCH_TRAP_H_


static inline void __TRAP_COND(int trap, int cond)
{
	asm volatile ("tne	$0, %1, %0" : : "i" (trap), "r" (cond));
}

static inline void __TRAP(int trap)
{
	asm volatile ("teq	$0, $0, %0" : : "i" (trap));
}

#endif
