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

#ifdef	NEED___CURRENT_THREAD
struct thread *__current_thread;
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

static void __sched_init_idle(uint cpu, struct thread *idle)
{
	runq.idle_thread[cpu] = idle;
}


struct thread* __switch_to(struct thread* prev, struct thread* next);
static struct thread* context_switch(struct thread* prev, struct thread* next)
{
	prev = __switch_to(prev, next);

	return prev;
}

static void enqueue_thread_locked(struct run_queue* rq, struct thread* t)
{
	unsigned prio = t->priority;

	if (!t->run_list.next) {
		dlist_add_tail(&rq->queues[prio], &t->run_list);
		rq->nr_running++;
	}
	rq->bitmap |= 1 << prio;
}

static void __thread_activate(struct thread* t)
{
	struct thread* curr = get_current_thread();
	struct run_queue* rq = &runq;

	t->state = THREAD_STATE_READY;
	if (t->priority >= curr->priority)
		thread_need_resched_set(curr);

	lock_acq_irq(&rq->lock);
	enqueue_thread_locked(rq, t);
	lock_rel_irq(&rq->lock);
}

static struct thread *dequeue_thread_locked(struct run_queue* rq, uint prio)
{
	struct dlist_head* q = &rq->queues[prio];
	struct thread *t;

	t = __dlist_pop_entry(q, struct thread, run_list);
	rq->nr_running--;
	t->run_list.next = NULL;
	if (dlist_is_empty(q))
		rq->bitmap &= ~(1 << prio);

	return t;
}

static void dump_rq(const char *ctxt, int locked)
{
	struct run_queue* rq = &runq;
	struct thread* t;
	uint n = 0;
	int prio;

	if (!locked)
		lock_acq_irq(&rq->lock);

	printf("dump rq @ %s on cpu %d:\n", ctxt, __coreid());
	printf("\tnr = %u\n", rq->nr_running);
	printf("\tbitmap= %08lX (%lb)\n", rq->bitmap, rq->bitmap);

	for (prio=0; prio < CONFIG_NR_THREAD_PRIORITY; prio++) {
		if (!dlist_is_empty(&rq->queues[prio])) {
			printf("    rq[%d]:\n", prio);
			dlist_foreach_entry(t, &rq->queues[prio], run_list) {
				printf("\t%p\n", t);
				n++;
			}
		}
	}

	if (n != rq->nr_running)
		pr_err("rq integrity: nr %d != %d\n", rq->nr_running, n);
	if ((n == 0) != (rq->bitmap == 0))
		pr_err("rq integrity: nr %d, bitmap: %08x\n", n, rq->bitmap);
	printf("-------- (%s)\n", ctxt);

	if (!locked)
		lock_rel_irq(&rq->lock);
}

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
 * Try to run queued thread on a idle CPU:
 *	if we have more than one thread on the queue
 *	and there is an idle cpu
 *	then ask this cpu the reschedule
 *
 * FIXME: This should be part of some sort of PM policy:
 *	when is it worth to wake a CPU?
 *
 * We acccess the run queue without locking it, it's fine.
 * At worst we wake up a cpu for nothing or we miss an opportunity
 * to run a process on another cpu but everything is still OK.
 */
static bool activate_idle_cpu(struct run_queue* rq)
{
	unsigned int cpu;
	unsigned int i;

	// Notify the first idle CPU.
	// This will strongly favour the CPUs with the lowest ID,
	// which is fine and will let the highest ones in low-power.
	// FIXME: use a bitmap for rq->idle?
	cpu = __coreid();
	for (i = 0; i < NR_CPUS; i++) {
		if (i == cpu)
			continue;
		if (!cpu_is_idle(rq, i))
			continue;

		smp_ipi_schedule_one(i);

		// We could do this until nr_running == 0, but
		// - the cpu just notified will take one thread
		//   and itself try this again
		return true;
	}

	return false;
}
#else
static void smp_set_idle(struct run_queue *rq, unsigned int cpu, int val)
{
}

static bool activate_idle_cpu(struct run_queue* rq)
{
	return false;
}
#endif

static void __schedule(void)
{
	struct thread* prev;
	struct thread* next;
	struct run_queue* rq = &runq;
	unsigned int cpu;

	lock_acq_irq(&rq->lock);
	cpu = __coreid();
	prev = get_current_thread();
	thread_need_resched_clear(prev);

	if (rq->bitmap != 0) {
		int prio = bitop_fmsb(rq->bitmap);

		next = dequeue_thread_locked(rq, prio);
		smp_set_idle(rq, cpu, 0);
	} else if (prev->state == THREAD_STATE_READY) {
		next = prev;
	} else {
		next = rq->idle_thread[cpu];
		smp_set_idle(rq, cpu, 1);
	}

	if (prev != next) {
		if (prev->state == THREAD_STATE_READY)
			enqueue_thread_locked(rq, prev);
		set_current_thread(next);
		prev = context_switch(prev, next);
		barrier();
	}
	lock_rel_irq(&rq->lock);

	if (rq->nr_running)
		activate_idle_cpu(rq);
}

static void __sched_start_thread(struct thread *t)
{
	struct run_queue* rq = &runq;

	// This release the lock taken by thread_schedule() the first time
	// this thread is scheduled.
	// This is needed because normally the context is saved & loaded by
	// thread_schedule() but the first context is loaded with the starting
	// function & data and thsu the first schedule make the thread to NOT
	// returns from schedule() (only the previous thread call it) but
	// instead executing its starting function.
	lock_rel_irq(&rq->lock);
}

static int __sched_wakeup(struct thread* t)
{
	if (t->state == THREAD_STATE_READY)
		return 0;		// FIXME: should never happen?

	__thread_activate(t);
	return 1;
}
/******************************************************************************/

void thread_schedule(void)
{
	do {
		__schedule();
	} while (need_resched());
}


void __sched_start(uint cpu)
{
	struct thread* t = get_current_thread();
	// t == init_thread

	t->state = THREAD_STATE_IDLE;
	t->priority   = 0;
	t->flags = TIF_NEED_RESCHED;

	__sched_init_idle(cpu, t);
}

/**
* Thread entry point.
*
* Called by thread_entry() which do the arch specific part.
*/
void __thread_start(void (*fun)(void *data), void *data)
{
	struct thread *t = get_current_thread();

	__sched_start_thread(t);

	// this is where the thread will spend all its lifetime
	fun(data);

	// the thread has exited

	t = get_current_thread();
	t->state = THREAD_STATE_EXITED;
	thread_schedule();

	// FIXME: free stack
	// thread_join()?
}

void thread_start(struct thread* t)
{
	// FIXME: need to keep count of the level of suspendness

	__thread_activate(t);
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
	__sched_wakeup(t);
}
