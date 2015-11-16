#ifndef	_64BIT_H_
#define	_64BIT_H_


static inline u64 mk_u64(u32 hi, u32 lo)
{
	/*
	 * This function only exist because gcc generate horrible code
	 * when using a cast-shift-{or,add} combo (at least on ARM).
	 */
	union {
		unsigned long long	hilo;
		struct {
#if	__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
			unsigned int lo, hi;
#elif	__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			unsigned int hi, lo;
#endif
		};
	}  res = { .lo = lo, .hi = hi, };

	return res.hilo;
}

#endif
