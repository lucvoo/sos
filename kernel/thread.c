#include <thread.h>
#include <sched.h>
#include <stddef.h>


int thread_create(struct thread* t, int priority, void (*entry_func)(void*), void* entry_data)
{
	t->priority	= priority;
	t->flags	= TIF_NEED_RESCHED;
	t->state	= THREAD_STATE_SLEEPING;

	t->run_list.next = NULL;

	thread_load_context(t, entry_func, entry_data);

	return 0;
}
