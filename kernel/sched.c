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
	struct thread*		curr;
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


extern void __switch_to(struct thread* prev, struct thread* next);
static struct thread* context_switch(struct thread* prev, struct thread* next)
{
	__switch_to(prev, next);

	return prev;
}

void thread_schedule(void)
{
	struct thread* prev;
	struct thread* next;
	struct run_queue* rq = &runq;

	lock_acq_irq(&rq->lock);
	if (rq->bitmap == 0)
		next = rq->idle;
	else {
		int prio = __builtin_ffsl(rq->bitmap) -1;
		struct dlist* q = &rq->queues[prio];
		next = dlist_entry(q->next, struct thread, run_list);
	}

	prev = rq->curr;		// FIXME
	if (prev != next) {
		prev = context_switch(prev, next);
		barrier();
		rq->curr = next;
	}
	lock_rel_irq(&rq->lock);
}
