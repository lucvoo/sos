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
#include <smp.h>


#if CONFIG_NR_THREAD_PRIORITY > BITS_PER_LONG
#error	CONFIG_NR_THREAD_PRIORITY must not be greater than BITS_PER_LONG
#endif


struct run_queue {
	struct lock		lock;
	struct dlist_head	queues[CONFIG_NR_THREAD_PRIORITY];
	unsigned long		bitmap;
	unsigned int		nr_running;
	struct thread*		idle_thread[NR_CPUS];
	unsigned int		idle[NR_CPUS];
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

static void enqueue_thread_locked(struct thread* t, struct run_queue* rq)
{
	unsigned prio = t->priority;

	if (!t->run_list.next) {
		dlist_add_tail(&rq->queues[prio], &t->run_list);
		rq->nr_running++;
	}
	rq->bitmap |= 1 << prio;
}

static void enqueue_thread(struct thread* t, struct run_queue* rq)
{
	lock_acq_irq(&rq->lock);
	enqueue_thread_locked(t, rq);
	lock_rel_irq(&rq->lock);
}

static void dequeue_thread_adjust(struct thread* t, struct run_queue* rq, unsigned int prio)
{
	t->run_list.next = NULL;
	if (dlist_is_empty(&rq->queues[prio]))
		rq->bitmap &= ~(1 << prio);
}

static void dequeue_thread_locked(struct thread* t, struct run_queue* rq)
{
	unsigned prio = t->priority;

	dlist_del(&t->run_list);
	rq->nr_running--;
	dequeue_thread_adjust(t, rq, prio);
}

#if 0
static void dump_rq(const char *ctxt)
{
	struct run_queue* rq = &runq;
	struct thread* t;
	int prio;
	printf("dump rq @ %s:\n", ctxt);
	printf("\tnr = %u\n", rq->nr_running);
	printf("\tidle_thread= %p\n", rq->idle_thread[cpu]);
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

#ifdef CONFIG_SMP
static void smp_set_idle(struct run_queue *rq, unsigned int cpu, int val)
{
	rq->idle[cpu] = val;
}

static inline int cpu_is_idle(struct run_queue *rq, unsigned int cpu)
{
	// Must be called with @rq->lock hold

	return rq->idle[cpu] != 0;
}

/*
 * Stupid load balancing:
 *	if we have more than one thread on the queue
 *	and there is an idle cpu
 *	then ask this cpu the reschedule
 * We acccess the run queue without locking it, it's fine.
 * At worst we wake up a cpu for nothing or we miss an opportunity
 * to run a process on another cpu but everything is still OK.
 */
static void smp_load_balancing(struct run_queue* rq)
{
	unsigned int cpu;
	unsigned int i;
	unsigned int n;

	if (NR_CPUS == 1)
		return;

	n = rq->nr_running;		// Quick test, with rq unlocked
	if (n == 0)
		return;

	lock_acq_irq(&rq->lock);
	n = rq->nr_running;
	if (n == 0)			// Test again with the lock hold.
		goto end;

	// Notify the first idle CPU.
	// This will strongly favour the CPUs with the lowest ID,
	// which is fine and will let the highest ones in low-power.
	cpu = __coreid();
	for (i = 0; i < NR_CPUS; i++) {
		if (i == cpu)
			continue;
		if (!cpu_is_idle(rq, i))
			continue;

		smp_ipi_schedule_one(i);

		// We could do this until nr_running == 0, but
		// - the cpu just notified will take one thread
		//   and itself try to balance
		// - better to release the rq lock ASAP
		break;
	}
end:
	lock_rel_irq(&rq->lock);
}
#else
static void smp_set_idle(struct run_queue *rq, unsigned int cpu, int val)
{
}

static void smp_load_balancing(struct run_queue* rq)
{
}
#endif

void thread_schedule(void)
{
	struct thread* prev;
	struct thread* next;
	struct run_queue* rq = &runq;
	unsigned int cpu;

need_resched:
	lock_acq_irq(&rq->lock);
	cpu = __coreid();
	prev = get_current_thread();
	thread_need_resched_clear(prev);

	if (rq->bitmap != 0) {
		int prio = bitop_fmsb(rq->bitmap);
		struct dlist_head* q = &rq->queues[prio];
		next = dlist_pop_entry(q, struct thread, run_list);
		rq->nr_running--;
		dequeue_thread_adjust(next, rq, prio);
		smp_set_idle(rq, cpu, 0);
	} else if (prev->state == THREAD_STATE_READY) {
		next = prev;
	} else {
		next = rq->idle_thread[cpu];
		smp_set_idle(rq, cpu, 1);
	}

	if (prev != next) {
		if (prev->state == THREAD_STATE_READY)
			enqueue_thread_locked(prev, rq);
		set_current_thread(next);
		prev = context_switch(prev, next);
		barrier();
	}
	lock_rel_irq(&rq->lock);

	smp_load_balancing(rq);

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
	runq.idle_thread[__coreid()] = t;
	t->state = THREAD_STATE_IDLE;
	t->priority   = 0;
	t->state      = 0;
	t->flags      = 0;
	thread_need_resched_set(t);
}

/**
* Thread entry point.
*
* Called by thread_entry() which do the arch specific part.
*/
void __thread_start(void (*fun)(void *data), void *data)
{
	struct run_queue* rq = &runq;
	struct thread *t;

	// This release the lock taken by thread_schedule() the first time
	// this thread is scheduled.
	// This is needed because normally the context is saved & loaded by
	// thread_schedule() but the first context is loaded with the starting
	// function & data and thsu the first schedule make the thread to NOT
	// returns from schedule() (only the previous thread call it) but
	// instead executing its starting function.
	lock_rel_irq(&rq->lock);

	fun(data);

	t = get_current_thread();
	t->state = THREAD_STATE_EXITED;
	thread_schedule();

	// FIXME: free stack
	// thread_join()?
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
