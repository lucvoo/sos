#ifndef	_ARCH_TRAP_H_
#define	_ARCH_TRAP_H_


static inline void __TRAP(int trap)
{
	asm volatile("brk	%0" :: "n" (trap));
}

#endif
