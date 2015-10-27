#ifndef	_ARCH_XCHG_H_
#define	_ARCH_XCHG_H_


static inline int __cmpxchg_weak(volatile uint *ptr, uint old, uint new)
{
	asm volatile goto("\n"
	"	ll	$1, %[ptr]\n"
	"	bne	$1, %[old], %l[failed]\n"
	"	 nop\n"
	"	move	$1, %[new]\n"
	"	sc	$1, %[ptr]\n"
	"	beqz	$1, %l[failed]\n"
	"	 nop\n"
	:
	: [ptr] "m" (*ptr), [old] "r" (old), [new] "r" (new)
	: "memory"
	: failed);

	return 1;

failed:
	return 0;
}

#endif
