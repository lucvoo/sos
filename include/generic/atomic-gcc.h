#ifndef	_GENERIC_ATOMIC_GCC_H_
#define	_GENERIC_ATOMIC_GCC_H_

// use GCC's builtins

#define	__BARRIER_NONE	__ATOMIC_RELAXED
#define	__BARRIER_FULL	__ATOMIC_SEQ_CST

#ifdef	CONFIG_SMP
#define	__BARRIER_SMP	__BARRIER_FULL
#else
#define	__BARRIER_SMP	__BARRIER_NONE
#endif

static inline int atomic_get(const atomic_t *ptr)
{
	return __atomic_load_n(&ptr->val, __BARRIER_NONE);
}

static inline void atomic_set(atomic_t *ptr, int val)
{
	__atomic_store_n(&ptr->val, val, __BARRIER_NONE);
}


static inline void atomic_add(atomic_t *ptr, int val)
{
	__atomic_add_fetch(&ptr->val, val, __BARRIER_NONE);
}

static inline void atomic_sub(atomic_t *ptr, int val)
{
	__atomic_sub_fetch(&ptr->val, val, __BARRIER_NONE);
}

static inline int atomic_add_return(atomic_t *ptr, int val)
{
	int new;

	new = __atomic_add_fetch(&ptr->val, val, __BARRIER_SMP);
	return new;
}

static inline int atomic_sub_return(atomic_t *ptr, int val)
{
	int new;

	new = __atomic_sub_fetch(&ptr->val, val, __BARRIER_SMP);
	return new;
}


#define	atomic_inc(p)		atomic_add(p, 1)
#define	atomic_dec(p)		atomic_sub(p, 1)

#define	atomic_inc_return(p)	atomic_add_return(p, 1)
#define	atomic_dec_return(p)	atomic_sub_return(p, 1)
#define	atomic_inc_test(p)	(atomic_add_return(p, 1) == 0)
#define	atomic_dec_test(p)	(atomic_sub_return(p, 1) == 0)
#define	atomic_sub_test(p, v)	(atomic_sub_return(p, v) == 0)


#define	atomic_tst(ptr, msk)	({ msk & __atomic_load_n(ptr, __BARRIER_SMP); })
#define	atomic_clr(ptr, msk)	({ msk & __atomic_fetch_and(ptr,~msk, __BARRIER_SMP); })
#define	atomic_or(ptr, msk)	({ msk & __atomic_fetch_or( ptr, msk, __BARRIER_SMP); })
#define	atomic_xor(ptr, msk)	({ msk & __atomic_fetch_xor(ptr, msk, __BARRIER_SMP); })

#endif
