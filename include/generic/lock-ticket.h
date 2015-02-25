#ifndef	_GENERIC_LOCK_H_
#define	_GENERIC_LOCK_H_

#include <arch/atomic.h>
#include <types/kint.h>
#include <barrier.h>


struct lock {
	u16 curr;
	u16 next;
} __aligned(4);

#define	LOCK_INIT	{ .curr = 0, .next = 0, }

#define	TLOCK_BITS	16
#define	TLOCK_MASK	((1 << TLOCK_BITS) - 1)

static inline void __lock_acq(struct lock *l)
{
	u32 ticket;
	unsigned int mine;
	unsigned int curr;

	ticket = __atomic_fetch_add((u32 *)l, 1 << TLOCK_BITS, __BARRIER_NONE);
	mine = ticket >> TLOCK_BITS;
	curr = ticket & TLOCK_MASK;

	while (curr != mine) {
		 curr = mb_get(l->curr);
	}

	mb_smp();
}

static inline void __lock_rel(struct lock *l)
{
	mb_smp();
	l->curr++;
}

#endif
