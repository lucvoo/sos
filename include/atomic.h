#ifndef	_ATOMIC_H_
#define	_ATOMIC_H_

#include <arch/atomic.h>


static inline int atomic_xchg(atomic_t *ptr, unsigned int new)
{
	return __xchg(&ptr->val, new);
}

static inline int atomic_cmpxchg(atomic_t *ptr, unsigned int old, unsigned int new)
{
	return __cmpxchg(&ptr->val, old, new);
}

static inline int atomic_cmpxchg_weak(atomic_t *ptr, unsigned int old, unsigned int new)
{
	return __cmpxchg_weak(&ptr->val, old, new);
}

#endif
