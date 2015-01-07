#ifndef	_GENERIC_ATOMIC_GCC_H_
#define	_GENERIC_ATOMIC_GCC_H_

// use GCC's builtins


static inline int atomic_get(const atomic_t *ptr)
{
	return __atomic_load_n(&ptr->val, __ATOMIC_RELAXED);
}

static inline void atomic_set(atomic_t *ptr, int val)
{
	__atomic_store_n(&ptr->val, val, __ATOMIC_RELAXED);
}


static inline void atomic_add(atomic_t *ptr, int val)
{
	__atomic_add_fetch(&ptr->val, val, __ATOMIC_RELAXED);
}

static inline void atomic_sub(atomic_t *ptr, int val)
{
	__atomic_sub_fetch(&ptr->val, val, __ATOMIC_RELAXED);
}

static inline int atomic_add_return(atomic_t *ptr, int val)
{
	int new;

	// FIXME: this need a SMP barrier to satisfy Linux's semantic
	new = __atomic_add_fetch(&ptr->val, val, __ATOMIC_RELAXED);
	// FIXME: this need a SMP barrier to satisfy Linux's semantic
	return new;
}

static inline int atomic_sub_return(atomic_t *ptr, int val)
{
	int new;

	// FIXME: this need a SMP barrier to satisfy Linux's semantic
	new = __atomic_sub_fetch(&ptr->val, val, __ATOMIC_RELAXED);
	// FIXME: this need a SMP barrier to satisfy Linux's semantic
	return new;
}


#define	atomic_inc(p)		atomic_add(p, 1)
#define	atomic_dec(p)		atomic_sub(p, 1)

#define	atomic_inc_return(p)	atomic_add_return(p, 1)
#define	atomic_dec_return(p)	atomic_sub_return(p, 1)
#define	atomic_inc_test(p)	(atomic_add_return(p, 1) == 0)
#define	atomic_dec_test(p)	(atomic_sub_return(p, 1) == 0)
#define	atomic_sub_test(p, v)	(atomic_sub_return(p, v) == 0)


#include <generic/xchg-gcc.h>

static inline int atomic_xchg(atomic_t *ptr, unsigned int new)
{
	return __xchg(&ptr->val, new);
}

static inline int atomic_cmpxchg(atomic_t *ptr, unsigned int old, unsigned int new)
{
	return __cmpxchg(&ptr->val, old, new);
}


#define	atomic_clr(ptr, msk)	({ msk & __atomic_fetch_and(ptr,~msk, __ATOMIC_SEQ_CST); })
#define	atomic_or(ptr, msk)	({ msk & __atomic_fetch_or( ptr, msk, __ATOMIC_SEQ_CST); })
#define	atomic_xor(ptr, msk)	({ msk & __atomic_fetch_xor(ptr, msk, __ATOMIC_SEQ_CST); })

#endif
