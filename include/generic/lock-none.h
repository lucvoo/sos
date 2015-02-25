#ifndef	_GENERIC_LOCK_H_
#define	_GENERIC_LOCK_H_

#ifdef CONFIG_SMP
#error "SMP need a real implementation of locks"
#endif


struct lock {
};

#define LOCK_INIT	{ }

static inline void __lock_acq(struct lock *lock)
{
	(void) lock;
}

static inline void __lock_rel(struct lock *lock)
{
	(void) lock;
}

#endif
