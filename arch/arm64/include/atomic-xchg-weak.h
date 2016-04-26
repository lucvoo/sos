#ifndef	_ARCH_XCHG_H_
#define	_ARCH_XCHG_H_

static inline int __cmpxchg_weak(volatile uint *ptr, uint old, uint new)
{
	uint cur;
	int res;

	asm volatile("\n"
	"	ldxr	%w[cur], %[ptr]\n"
	"	cmp	%w[cur], %w[old]\n"
	"	cset	%w[res], ne\n"
	"	bne	1f\n"
	"	stxr	%w[res], %w[new], %[ptr]\n"
	"1:\n"
	: [res] "=&r" (res), [cur] "=&r" (cur), [ptr] "+Q" (*ptr)
	: [old]  "Ir" (old), [new] "r" (new)
	: "cc");

	return res == 0;
}

#endif
