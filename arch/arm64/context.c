#include <thread.h>
#include <sched.h>


static void thread_entry(void)
{
	struct thread* t = get_current_thread();
	struct cpu_context* ctxt = &t->cpu_context;
	void (*func)(void*) = (void *) ctxt->x[19];
	void *data = (void*) ctxt->x[20];

	__thread_start(func, data);
}


/*
 * We can't directly load ctxt->lr/x30 with __thread_start()'s address
 * and ctxt->x0 & x1 with __thread_start()'s args because
 * the cpu's pc, x0 & x1 will only be loaded at the first schedule()
 * and this only save/restore the non-call clobered registers
 * (ctxt->x0 & x1 doesn't even exist).
 * Thus the need for thead_entry() here above.
 */
void thread_load_context(struct thread* t, void (*func)(void*), void* data, void* stack_top)
{
	struct cpu_context* ctxt = &t->cpu_context;
	ulong *sp = stack_top;

	ctxt->x[19] = (ulong) func;
	ctxt->x[20] = (ulong) data;

	// mark the end of the call stack
	*--sp = 0;
	*--sp = (ulong) stack_top;
	ctxt->sp = (ulong)sp;

	ctxt->x[29] = (ulong) sp;		// FP
	ctxt->x[30] = (ulong) thread_entry;
}
