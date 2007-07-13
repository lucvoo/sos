#ifndef	_SCHED_H_
#define	_SCHED_H_

#include <thread.h>
#include <compiler.h>

static inline int need_resched(void)
{
	return unlikely(thread_flag_test(get_current_thread(), TIF_NEED_RESCHED));
}

#endif
