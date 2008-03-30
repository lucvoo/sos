#ifndef	_WAITQUEUE_H_
#define	_WAITQUEUE_H_

#include <types/waiter.h>
#include <types/waitqueue.h>
#include <dlist.h>
#include <lock.h>


struct thread;


void waitqueue_init(struct waitqueue* wq);
void waitqueue_wake_one(struct waitqueue* wq);


static inline void waitqueue_add(struct waitqueue* wq, struct waiter* w, struct thread* t)
{
	w->thread = t;
	dlist_add_tail(&wq->waiters, &w->node);
}

static inline void waitqueue_rem(struct waiter* w)
{
	dlist_del(&w->node);
}

static inline int waitqueue_empty(struct waitqueue* wq)
{
	return dlist_is_empty(&wq->waiters);
}

#endif
