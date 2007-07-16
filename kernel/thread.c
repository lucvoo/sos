#include <thread.h>


int thread_create(struct thread* t, int priority, void (*entry_func)(void*), void* entry_data)
{
	t->entry_point	= entry_func;
	t->entry_data	= entry_data;
	t->priority	= priority;
	t->flags	= TIF_NEED_RESCHED;
	t->state	= THREAD_STATE_SLEEPING;

	return 0;
}
