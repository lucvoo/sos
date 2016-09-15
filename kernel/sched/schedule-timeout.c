#include <sched.h>
#include <timer.h>


static void wakeup(void *data)
{
	struct thread *th = data;

	thread_wakeup(th);
}


long thread_schedule_timeout(unsigned long ticks)
{
	struct thread *th = get_current_thread();
	struct timer t;

	th->state = THREAD_STATE_SLEEPING;

	t.action = wakeup;
	t.data   = th;
	timer_add_rel(&t, ticks);

	thread_schedule();

	return 0;
}
