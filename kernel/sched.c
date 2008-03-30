#include <init.h>
#include <utils.h>
#include <dlist.h>
#include <lock.h>
#include <thread.h>
#include <sched.h>
#include <types.h>
#include <bitops/findbit.h>


#if CONFIG_NR_THREAD_PRIORITY > BITS_PER_LONG
#error	CONFIG_NR_THREAD_PRIORITY must not be greater than BITS_PER_LONG
#endif


struct run_queue {
	struct lock		lock;
	struct dlist		queues[CONFIG_NR_THREAD_PRIORITY];
	unsigned long		bitmap;
#ifndef	CONFIG_FIXED_STACKS
	struct thread*		curr;
#endif
	struct thread*		idle;
};

static struct run_queue runq;

static void __init sched_init(void)
{
	struct run_queue* rq = &runq;
	int i;

	for (i=ARRAY_SIZE(rq->queues); i--; ) {
		dlist_init(&rq->queues[i]);
	}
}
pure_initcall(sched_init);


struct thread* __switch_to(struct thread* prev, struct thread* next);
static struct thread* context_switch(struct thread* prev, struct thread* next)
{
	prev = __switch_to(prev, next);

	return prev;
}

static void enqueue_thread(struct thread* t, struct run_queue* rq)
{
	unsigned prio = t->priority;

	lock_acq_irq(&rq->lock);
	dlist_add_tail(&rq->queues[prio], &t->run_list);
	rq->bitmap |= 1 << prio;
	lock_rel_irq(&rq->lock);
}

static void dequeue_thread_locked(struct thread* t, struct run_queue* rq)
{
	unsigned prio = t->priority;

	dlist_del(&t->run_list);
	if (dlist_is_empty(&rq->queues[prio]))
		rq->bitmap &= ~(1 << prio);
}

void thread_schedule(void)
{
	struct thread* prev;
	struct thread* next;
	struct run_queue* rq = &runq;

need_resched:
	lock_acq_irq(&rq->lock);
	prev = get_current_thread();
	thread_need_resched_clear(prev);

	if (prev->state != THREAD_STATE_READY)
		dequeue_thread_locked(prev, rq);

	if (rq->bitmap == 0)
		next = rq->idle;
	else {
		int prio = bitop_fmsb(rq->bitmap);
		struct dlist* q = &rq->queues[prio];
		next = dlist_entry(q->next, struct thread, run_list);
	}

	if (prev != next) {
		prev = context_switch(prev, next);
		barrier();
#ifndef	CONFIG_FIXED_STACKS
		rq->curr = next;
#endif
	}
	lock_rel_irq(&rq->lock);

	if (thread_need_resched_test(prev))
		goto need_resched;
}

static int wake_up(struct thread* t)
{
	struct run_queue *rq = &runq;
	struct thread* curr = get_current_thread();
	int rc = 1;

	if (t->state == THREAD_STATE_READY)
		goto out;		// FIXME: should never happen?

	t->state = THREAD_STATE_READY;
	enqueue_thread(t, rq);
	if (t->priority >= curr->priority)
		thread_need_resched_set(curr);

	rc = 0;

out:
	return rc;
}


void _thread_scheduler_start(void)
{
	struct thread* t = get_current_thread();

	runq.idle = t;
	thread_need_resched_set(t);
}

void thread_start(struct thread* t)
{
	struct run_queue* rq = &runq;

	thread_need_resched_set(t);
	t->state = THREAD_STATE_READY;
	// FIXME: need to keep count of the level of suspendness

	enqueue_thread(t, rq);
}

void thread_yield(void)
{
	struct thread* t = get_current_thread();
	struct run_queue* rq = &runq;

	lock_acq_irq(&rq->lock);
	dlist_move_tail(&rq->queues[t->priority], &t->run_list);
	lock_rel_irq(&rq->lock);

	thread_schedule();
}

void thread_sleep(void)
{
	struct thread* t = get_current_thread();

	t->state = THREAD_STATE_SLEEPING;
	thread_schedule();
}

void thread_wakeup(struct thread* t)
{
	wake_up(t);
}
