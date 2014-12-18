#ifndef	_GENERIC_XCHG_GCC_H_
#define	_GENERIC_XCHG_GCC_H_

// use GCC's builtins

static inline unsigned int __xchg(unsigned int *p, unsigned int new)
{
	return __atomic_exchange_n(p, new, __ATOMIC_RELAXED);
}

static inline unsigned int __cmpxchg(unsigned int *p, unsigned int old, unsigned int new)
{
	return __atomic_compare_exchange_n(p, &old, new, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

#endif
