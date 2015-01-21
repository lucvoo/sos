#include <init.h>
#include <utils.h>
#include <dlist.h>
#include <lock.h>
#include <thread.h>
#include <sched.h>
#include <types.h>
#include <bitops/findbit.h>
#include <diag.h>
#include <barrier.h>


#if CONFIG_NR_THREAD_PRIORITY > BITS_PER_LONG
#error	CONFIG_NR_THREAD_PRIORITY must not be greater than BITS_PER_LONG
#endif


struct run_queue {
	struct lock		lock;
	struct dlist_head	queues[CONFIG_NR_THREAD_PRIORITY];
	unsigned long		bitmap;
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
	if (!t->run_list.next)
		dlist_add_tail(&rq->queues[prio], &t->run_list);
	rq->bitmap |= 1 << prio;
	lock_rel_irq(&rq->lock);
}

static void dequeue_thread_locked(struct thread* t, struct run_queue* rq)
{
	unsigned prio = t->priority;

	dlist_del(&t->run_list);
	t->run_list.next = NULL;
	if (dlist_is_empty(&rq->queues[prio]))
		rq->bitmap &= ~(1 << prio);
}

#if 0
static void dump_rq(const char *ctxt)
{
	struct run_queue* rq = &runq;
	struct thread* t;
	int prio;
	printf("dump rq @ %s:\n", ctxt);
	printf("\tidle= %p\n", rq->idle);
	printf("\tbitmap= %08lX (%lb)\n", rq->bitmap, rq->bitmap);

	for (prio=0; prio < CONFIG_NR_THREAD_PRIORITY; prio++) {
		if (!dlist_is_empty(&rq->queues[prio])) {
			printf("    rq[%d]:\n", prio);
			dlist_foreach_entry(t, &rq->queues[prio], run_list)
				printf("\t%p\n", t);
		}
	}
}
#endif

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
		struct dlist_head* q = &rq->queues[prio];
		next = dlist_peek_entry(q, struct thread, run_list);
	}

	if (prev != next) {
		set_current_thread(next);
		prev = context_switch(prev, next);
		barrier();
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

	// t == init_thread
	runq.idle = t;
	t->state = THREAD_STATE_READY;	// Need this otherwise, the first schedule will try to dequeue this thread, which was never queued ...
	t->priority   = 0;
	t->state      = 0;
	t->flags      = 0;
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
