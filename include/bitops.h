#ifndef	_BITOPS_H_
#define	_BITOPS_H_


#define BITOP_MASK(bit)		(1UL << ((bit) % (sizeof(long)*8)))
#define BITOP_WORD(bit)		((bit) / (sizeof(long)*8))


static inline void bitop_set(unsigned long *addr, unsigned int bit)
{
	addr[BITOP_WORD(bit)] |= BITOP_MASK(bit);
}

static inline void bitop_clr(unsigned long *addr, unsigned int bit)
{
	addr[BITOP_WORD(bit)] &= ~BITOP_MASK(bit);
}

static inline void bitop_inv(unsigned long *addr, unsigned int bit)
{
	addr[BITOP_WORD(bit)] ^= BITOP_MASK(bit);
}

static inline unsigned long bitop_get(const unsigned long *addr, unsigned int bit)
{
	return addr[BITOP_WORD(bit)] & BITOP_MASK(bit);
}

#endif
