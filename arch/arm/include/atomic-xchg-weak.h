#ifndef	_ARCH_XCHG_H_
#define	_ARCH_XCHG_H_

#include <arch/assembly.h>


static inline int __cmpxchg_weak(volatile uint *ptr, uint old, uint new)
{
	uint cur;
	int res;

	asm volatile("\n"
	"	ldrex	%[cur], %[ptr]\n"
	"	teq	%[cur], %[old]\n"
	"	movne	%[res], #1\n"
	"	strexeq	%[res], %[new], %[ptr]\n"
	: [res] "=&r" (res), [cur] "=&r" (cur), [ptr] "+" LLSC_MODE (*ptr)
	: [old]  "Ir" (old), [new] "r" (new)
	: "cc");

	return res == 0;
}

#endif
