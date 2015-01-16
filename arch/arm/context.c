#include <thread.h>


static void thread_entry(void)
{
	struct thread* t = get_current_thread();
	struct cpu_context* ctxt = &t->cpu_context;
	void (*func)(void*) = (void *) ctxt->r4;
	void *data = (void*) ctxt->r5;

	func(data);
}

void thread_load_context(struct thread* t, void (*func)(void*), void* data)
{
	struct cpu_context* ctxt = &t->cpu_context;
	unsigned long stack = (unsigned long) t;

	ctxt->r4 = (unsigned long) func;
	ctxt->r5 = (unsigned long) data;
	ctxt->fp = 0;

	ctxt->sp = stack + THREAD_SIZE - 8;
	ctxt->pc = (unsigned long) thread_entry;
}
