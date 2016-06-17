#ifndef	_GENERIC_ATOMIC_NOT_H_
#define	_GENERIC_ATOMIC_NOT_H_

#include <types/atomic.h>

#define __atomic_val(p)         (p)->val

static inline int atomic_get(const atomic_t *ptr)
{
	return ptr->val;
}

static inline void atomic_set(atomic_t *ptr, int val)
{
	ptr->val = val;
}


static inline void atomic_add(atomic_t *ptr, int val)
{
	ptr->val += val;
}

static inline void atomic_sub(atomic_t *ptr, int val)
{
	ptr->val -= val;
}

static inline int atomic_add_return(atomic_t *ptr, int val)
{
	return ptr->val += val;
}

static inline int atomic_sub_return(atomic_t *ptr, int val)
{
	return ptr->val -= val;
}


#define	atomic_inc(p)		atomic_add(p, 1)
#define	atomic_dec(p)		atomic_sub(p, 1)

#define	atomic_inc_return(p)	atomic_add_return(p, 1)
#define	atomic_dec_return(p)	atomic_sub_return(p, 1)
#define	atomic_inc_test(p)	(atomic_add_return(p, 1) == 0)
#define	atomic_dec_test(p)	(atomic_sub_return(p, 1) == 0)
#define	atomic_sub_test(p, v)	(atomic_sub_return(p, v) == 0)


#define	atomic_tst(ptr, msk)	({ msk & *ptr; })
#define	atomic_clr(ptr, msk)	({ msk & (*ptr &= ~msk); })
#define	atomic_or(ptr, msk)	({ msk & (*ptr |=  msk); })
#define	atomic_xor(ptr, msk)	({ msk & (*ptr ^=  msk); })


static inline unsigned int __xchg(unsigned int *p, unsigned int new)
{
	uint cur = *p;

	*p = new;
	return cur;
}

static inline unsigned int __cmpxchg(unsigned int *p, unsigned int old, unsigned int new)
{
	uint cur = *p;

	if (cur == old) {
		*p = new;
		return 1;
	} else {
		return 0;
	}
}

#endif
