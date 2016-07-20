#ifndef	_ARCH_TRAP_H_
#define	_ARCH_TRAP_H_


#define	_undef(N)	__asm__ __volatile__("udf %0" :: "n" (N))

static inline void __TRAP(int trap)
{
	_undef(trap);
}

#endif
