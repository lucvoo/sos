#include <init.h>
#include <utils.h>
#include <dlist.h>
#include <lock.h>
#include <thread.h>


#if CONFIG_NR_THREAD_PRIORITY > 32
#error	CONFIG_NR_THREAD_PRIORITY must not be greater than 32
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

void thread_schedule(void)
{
	struct thread* prev;
	struct thread* next;
	struct run_queue* rq = &runq;

need_resched:
	lock_acq_irq(&rq->lock);
	if (rq->bitmap == 0)
		next = rq->idle;
	else {
		int prio = __builtin_ffsl(rq->bitmap) -1;
		struct dlist* q = &rq->queues[prio];
		next = dlist_entry(q->next, struct thread, run_list);
	}

	prev = get_current_thread();
	thread_need_resched_clear(prev);
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
