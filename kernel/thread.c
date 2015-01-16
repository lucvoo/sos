#include <thread.h>
#include <sched.h>
#include <stddef.h>


static void thread_entry(void)
{
	struct thread* t = get_current_thread();
	void (*func)(void*);
	void* data;

	thread_need_resched_clear(t);
	thread_initial_context(t, func, data);

	func(data);
}

int thread_create(struct thread* t, int priority, void (*entry_func)(void*), void* entry_data)
{
	t->priority	= priority;
	t->flags	= TIF_NEED_RESCHED;
	t->state	= THREAD_STATE_SLEEPING;

	t->run_list.next = NULL;

	thread_load_context(t, entry_func, entry_data, thread_entry);

	return 0;
}
