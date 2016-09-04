#ifndef	_64BIT_H_
#define	_64BIT_H_


typedef union {
	struct {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	u32	hi;
#endif
	u32	lo;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	u32	hi;
#endif
	};
	u64	hl;
} u2x32_t;


static inline u64 mk_u64(u32 hi, u32 lo)
{
	// This function only exists because gcc generate horrible code
	// when using a cast-shift-{or,add} combo (at least on ARM).
	u2x32_t res = { .lo = lo, .hi = hi, };

	return res.hl;
}

#endif
