#ifndef	_LOCK_H_
#define	_LOCK_H_


#include <generic/lock-none.h>


#define LOCK_UNLOCKED	(struct lock) LOCK_INIT
#define DEFINE_LOCK(x)	struct lock x = LOCK_UNLOCKED

static inline void lock_init(struct lock *lock)
{
	*lock = LOCK_UNLOCKED;
}


#include <irqflags.h>

static inline void lock_acq(struct lock *lock)
{
	__lock_acq(lock);
}

static inline void lock_acq_irq(struct lock *lock)
{
	__local_irq_dis();
	__lock_acq(lock);
}

static inline unsigned long __must_check lock_acq_save(struct lock *lock)
{
	unsigned long flags;

	flags = __local_irq_save();
	__lock_acq(lock);

	return flags;
}


static inline void lock_rel(struct lock *lock)
{
	__lock_rel(lock);
}

static inline void lock_rel_irq(struct lock *lock)
{
	__lock_rel(lock);
	__local_irq_ena();
}

static inline void lock_rel_rest(struct lock *lock, unsigned long flags)
{
	__lock_rel(lock);
	__local_irq_rest(flags);
}

#endif
