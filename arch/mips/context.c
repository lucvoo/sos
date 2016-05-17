#include <thread.h>
#include <sched.h>


static void thread_entry(void)
{
	struct thread* t = get_current_thread();
	struct cpu_context* ctxt = &t->cpu_context;
	void (*func)(void*) = (void *) ctxt->s[0];
	void *data = (void*) ctxt->s[1];

	__thread_start(func, data);
}

/*
 * We can't directly load ctxt->pc with __thread_start()'s address
 * and ctxt->a0 & a1 with __thread_start()'s args because
 * the cpu's pc, a0 & a1 will only be loaded at the first schedule()
 * and this only save/restore the non-call clobered registers
 * (ctxt->a0 & a1 doesn't even extist).
 * Thus the need for thead_entry() here above.
 */
void thread_load_context(struct thread* t, void (*func)(void*), void* data)
{
	struct cpu_context* ctxt = &t->cpu_context;

	ctxt->s[0] = (unsigned long) func;
	ctxt->s[1] = (unsigned long) data;
	ctxt->fp = 0;

	ctxt->sp = thread_get_stack_top(t) - 8;
	ctxt->ra = (unsigned long) thread_entry;
}
