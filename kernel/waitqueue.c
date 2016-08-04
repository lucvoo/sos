#include <waitqueue.h>

#include <dlist.h>
#include <sched.h>


static inline void dump_wq(struct waitqueue* wq)
{
	struct waiter *w;

	dlist_foreach_entry(w, &wq->waiters, node) {
		printf("\tw: %p.%p\n", w, w->thread);
	}
}


void waitqueue_init(struct waitqueue* wq)
{
	dlist_init(&wq->waiters);
	lock_init(&wq->lock);
}


void waitqueue_wake_one(struct waitqueue* wq)
{
	struct waiter* w;

	w = dlist_pop_entry(&wq->waiters, struct waiter, node);

	// FIXME: w could be NULL or its thread already awake

	thread_wakeup(w->thread);
}
