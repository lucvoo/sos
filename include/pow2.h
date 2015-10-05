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

#endif
