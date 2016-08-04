#ifndef	_SEMAPHORE_H_
#define	_SEMAPHORE_H_

#include <atomic.h>
#include <waitqueue.h>


struct semaphore {
	struct waitqueue	wq;
	atomic_t		count;
};


extern void __semaphore_wait_slowpath(struct semaphore* sem);

void semaphore_init(struct semaphore* sem, int val);

static inline void semaphore_wait(struct semaphore* sem)
{
	int val;

	val = atomic_sub_return(&sem->count, 1);
	if (unlikely(val < 0))
		 __semaphore_wait_slowpath(sem);
}

static inline void semaphore_post(struct semaphore* sem)
{
	int val;

	val = atomic_add_return(&sem->count, 1);
	if (unlikely(val <= 0))
		waitqueue_wake_one(&sem->wq);
}


#endif
