#ifndef	_MUTEX_H_
#define	_MUTEX_H_

#include <atomic.h>
#include <types/waitqueue.h>


struct mutex {
	atomic_t		state; // 0: unlocked, 1: locked, no waiters, >1: waiters
	struct waitqueue	wq;
};


void mutex_init(struct mutex* mtx, int val);
int mutex_try(struct mutex* mtx);
void mutex_get(struct mutex* mtx);
void mutex_put(struct mutex* mtx);

#endif
