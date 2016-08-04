#include <waitqueue.h>

#include <dlist.h>
#include <sched.h>
#include <lock.h>
#include <bug.h>


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
	ulong flags;

	flags = lock_acq_save(&wq->lock);
	w = dlist_pop_entry(&wq->waiters, struct waiter, node);
	lock_rel_rest(&wq->lock, flags);

	BUG_ON(!w);

	// FIXME: could the thread be already awake?
	thread_wakeup(w->thread);
}
