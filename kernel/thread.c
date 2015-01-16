#include <thread.h>
#include <sched.h>
#include <stddef.h>


int thread_create(struct thread* t, int prio, void (*func)(void*), void* data)
{
	t->priority	= prio;
	t->flags	= TIF_NEED_RESCHED;
	t->state	= THREAD_STATE_SLEEPING;

	t->run_list.next = NULL;

	thread_load_context(t, func, data);

	return 0;
}
