#ifndef	_LOCK_H_
#define	_LOCK_H_


#ifndef CONFIG_SMP
#include <generic/lock-none.h>
#else
#include <generic/lock-ticket.h>

void _lock_acq(struct lock *lock);
void _lock_rel(struct lock *lock);
#endif


#define LOCK_UNLOCKED	(struct lock) LOCK_INIT
#define DEFINE_LOCK(x)	struct lock x = LOCK_UNLOCKED

static inline void lock_init(struct lock *lock)
{
	*lock = LOCK_UNLOCKED;
}

static inline void lock_acq(struct lock *lock)
{
	_lock_acq(lock);
}

static inline void lock_rel(struct lock *lock)
{
	_lock_rel(lock);
}


#include <irqflags.h>

static inline void lock_acq_irq(struct lock *lock)
{
	__local_irq_dis();
	_lock_acq(lock);
}

static inline void lock_rel_irq(struct lock *lock)
{
	_lock_rel(lock);
	__local_irq_ena();
}


static inline unsigned long __must_check lock_acq_save(struct lock *lock)
{
	unsigned long flags;

	flags = __local_irq_save();
	_lock_acq(lock);

	return flags;
}

static inline void lock_rel_rest(struct lock *lock, unsigned long flags)
{
	_lock_rel(lock);
	__local_irq_rest(flags);
}

#endif
