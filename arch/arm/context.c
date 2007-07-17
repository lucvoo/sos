#include <thread.h>


void thread_load_context(struct thread* t, void (*func)(void*), void* data, void (*entry)(void))
{
	struct cpu_context* ctxt = &t->cpu_context;
	unsigned long stack = (unsigned long) t;

	ctxt->r4 = (unsigned long) func;
	ctxt->r5 = (unsigned long) data;
	ctxt->fp = 0;

	ctxt->sp = stack + THREAD_SIZE - 8;
	ctxt->pc = (unsigned long) entry;
}
