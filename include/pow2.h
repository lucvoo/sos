#ifndef	_POW2_H_
#define	_POW2_H_


static inline bool is_pow2(ulong val)
{
	if (!val)
		return 0;

	return ((val & (val - 1)) == 0);
}


/**
 * log_pow2 - returns the "order" of an exact power of 2
 * @val
 *
 * returns: -1 if @val is not an exact power of 2
 *          otherwise returns 'n' such that @val == (1 << n)
 */
static inline int log_pow2(ulong val)
{
	int n = __builtin_ffsl(val) - 1;

	if (n < 0)
		return -1;

	if (val != (1U << n))
		return -1;

	return n;
}

/**
 * pow2_roundup - compute the next highest power of two
 * @val
 *
 * notes: returns 0 if val was 0
 * taken from http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 */
static inline ulong pow2_roundup(ulong val)
{
	val--;
	val |= val >> 1;
	val |= val >> 2;
	val |= val >> 4;
	val |= val >> 8;
	val |= val >> 16;
#if CONFIG_BITS == 64
	val |= val >> 32;
#endif

	return val + 1;;
}

#endif
