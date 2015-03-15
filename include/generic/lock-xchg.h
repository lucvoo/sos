#ifndef	_GENERIC_LOCK_H_
#define	_GENERIC_LOCK_H_

#include <arch/atomic.h>
#include <barrier.h>


struct lock {
	int	lock;
};

#define	LOCK_INIT	{ .lock = 0, }


#define	LOCK_FREE	0
#define	LOCK_BUSY	1

static inline void __lock_acq(struct lock *l)
{

	while (1) {
		unsigned int old;

		old = __atomic_exchange_n(&l->lock, LOCK_BUSY, __BARRIER_NONE);
		if (old == LOCK_FREE)
			break;

		while (mb_get(l->lock) == LOCK_BUSY)
			;
	}

	mb_smp();
}

static inline void __lock_rel(struct lock *l)
{
	mb_smp();
	l->lock = LOCK_FREE;
}

#endif
