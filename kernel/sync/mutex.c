#include <mutex.h>
#include <waitqueue.h>
#include <thread.h>
#include <sched.h>

// Very simple but fair implementation of sleeping mutexes

// Things are quite easy:
//	1) we can only have a single unlocker at a time
//	2) mutex can't be used in interrupt context
//	3) we don't have to loop because we can't have false wakeup


int mutex_try(struct mutex* mtx)
{
	return atomic_cmpxchg(&mtx->state, 0, 1);
}


static __noinline void mutex_get_slowpath(struct mutex *mtx)
{
	struct thread* curr = get_current_thread();
	struct waitqueue *wq = &mtx->wq;
	struct waiter w;
	int new_state;

	// FIXME: we can have a small trylock loop here
	// More exactly we should spin here for as long as
	// the overhead of sleep & wakeup

	// prepare to wait
	lock_acq(&wq->lock);
	waitqueue_add(wq, &w, curr);
	lock_rel(&wq->lock);

	// double check
	new_state = atomic_inc_return(&mtx->state);
	if (new_state != 1) {
		// we're not the first: we have to wait
		curr->state = THREAD_STATE_SLEEPING;
		thread_schedule();
	}

	// cancle/finish
	// we have the lock: remove ourself from the queue
	lock_acq(&wq->lock);
	waitqueue_rem(&w);
	lock_rel(&wq->lock);
}

void mutex_get(struct mutex* mtx)
{
	// use weak cmpxchg() here to avoid any branches in the fast path
	if (unlikely(atomic_cmpxchg_weak(&mtx->state, 0, 1)))
		mutex_get_slowpath(mtx);

	// FIXME: the generated code is not as good as it could be
	//	(at least on ARM)
}


static __noinline void mutex_put_slowpath(struct mutex *mtx)
{
	struct waitqueue *wq = &mtx->wq;
	struct waiter *w;

	lock_acq(&wq->lock);
	w = dlist_peek_entry(&wq->waiters, struct waiter, node);
	lock_rel(&wq->lock);

	// FIXME: ASSERT(w != NULL);

	thread_wakeup(w->thread);
}

void mutex_put(struct mutex* mtx)
{
	if (unlikely(atomic_dec_return(&mtx->state) > 0))
		mutex_put_slowpath(mtx);
}
