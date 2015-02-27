#include <thread.h>
#include <sched.h>


static void thread_entry(void)
{
	struct thread* t = get_current_thread();
	struct cpu_context* ctxt = &t->cpu_context;
	void (*func)(void*) = (void *) ctxt->r4;
	void *data = (void*) ctxt->r5;

	__thread_start(func, data);
}

void thread_load_context(struct thread* t, void (*func)(void*), void* data)
{
	struct cpu_context* ctxt = &t->cpu_context;

	ctxt->r4 = (unsigned long) func;
	ctxt->r5 = (unsigned long) data;
	ctxt->fp = 0;

	ctxt->sp = thread_get_stack_top(t) - 8;
	ctxt->pc = (unsigned long) thread_entry;
}
