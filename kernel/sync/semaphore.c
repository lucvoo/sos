#include <semaphore.h>
#include <sched.h>


void semaphore_init(struct semaphore* sem, int val)
{
	__atomic_val(&sem->count) = val;
	waitqueue_init(&sem->wq);
}

void __semaphore_post_slowpath(struct semaphore* sem)
{
	unsigned long flags;

	flags = lock_acq_save(&sem->wq.lock);
	if (!waitqueue_empty(&sem->wq)) {
		waitqueue_wake_one(&sem->wq);
	}
	lock_rel_rest(&sem->wq.lock, flags);
}

void __semaphore_wait_slowpath(struct semaphore* sem)
{
	struct thread* curr = get_current_thread();
	struct waiter w;

	lock_acq(&sem->wq.lock);
	curr->state = THREAD_STATE_SLEEPING;
	waitqueue_add(&sem->wq, &w, curr);
	lock_rel(&sem->wq.lock);

	// FIXME: we *MUST* check the reason for the wakeup
	thread_schedule();
}
