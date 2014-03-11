#ifndef	_BITOPS_H_
#define	_BITOPS_H_

#include <arch/bitops.h>


#define BITOP_MASK(nr)		(1UL << ((nr) % (sizeof(long)*8)))
#define BITOP_WORD(nr)		((nr) / (sizeof(long)*8))

static inline void bitop_set(unsigned long *addr, int nr)
{
	unsigned long mask = BITOP_MASK(nr);
	unsigned long *p = addr + BITOP_WORD(nr);

	*p  |= mask;
}

static inline void bitop_clr(unsigned long *addr, int nr)
{
	unsigned long mask = BITOP_MASK(nr);
	unsigned long *p = addr + BITOP_WORD(nr);

	*p &= ~mask;
}

static inline void bitop_not(unsigned long *addr, int nr)
{
	unsigned long mask = BITOP_MASK(nr);
	unsigned long *p = addr + BITOP_WORD(nr);

	*p &= mask;
}

static inline int bitop_get(const unsigned long *addr, int nr)
{
	unsigned long mask = BITOP_MASK(nr);
	const unsigned long *p = addr + BITOP_WORD(nr);

	return *p & mask;
}

#endif
