#ifndef	_THREAD_H_
#define	_THREAD_H_

#include <arch/thread.h>
#include <dlist.h>

#define	THREAD_STATE_NEW	0
#define	THREAD_STATE_READY	1
#define	THREAD_STATE_SLEEPING	2
#define	THREAD_STATE_EXITED	3


typedef	void (*__entry_fun)(void*);

struct thread {
	struct cpu_context	cpu_context;
	void*			stack_base;
	unsigned long		stack_size;
	__entry_fun		entry_point;
	void*			entry_data;
	unsigned int		priority;
	struct dlist		run_list;
	int			state;
};


void thread_yield(void);

#endif
