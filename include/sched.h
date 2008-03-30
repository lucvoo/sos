#ifndef	_SCHED_H_
#define	_SCHED_H_

#include <thread.h>

static inline int thread_need_resched_test(const struct thread* t)
{
	return unlikely(thread_flag_test(t, TIF_NEED_RESCHED));
}

static inline void thread_need_resched_set(struct thread* t)
{
	thread_flag_set(t, TIF_NEED_RESCHED);
}

static inline void thread_need_resched_clear(struct thread* t)
{
	thread_flag_clear(t, TIF_NEED_RESCHED);
}

static inline int need_resched(void)
{
	return unlikely(thread_need_resched_test(get_current_thread()));
}


void	thread_schedule(void);
void	_thread_scheduler_start(void);
void	thread_sleep(void);
void	thread_wakeup(struct thread *t);


#endif
