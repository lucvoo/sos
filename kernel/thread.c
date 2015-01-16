#include <thread.h>
#include <sched.h>
#include <stddef.h>


int thread_create(struct thread* t, int prio, void (*func)(void*), void* data, void *stack, unsigned int size)
{
	t->priority	= prio;
	t->flags	= TIF_NEED_RESCHED;
	t->state	= THREAD_STATE_SLEEPING;

#ifndef	CONFIG_FIXED_STACKS
	t->stack_base = stack;
	t->stack_size = size;
#endif

	t->run_list.next = NULL;

	thread_load_context(t, func, data);

	return 0;
}
