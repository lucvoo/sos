#include <smp.h>
#include <thread.h>


extern struct thread init_thread[];

struct thread *__smp_start_idle_thread(void);
struct thread *__smp_start_idle_thread(void)
{
	struct thread *idle = &init_thread[__coreid()];

	set_current_thread(idle);

	return idle;
}
