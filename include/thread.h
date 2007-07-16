#ifndef	_THREAD_H_
#define	_THREAD_H_


#define	THREAD_STATE_NEW	0
#define	THREAD_STATE_READY	1
#define	THREAD_STATE_SLEEPING	2
#define	THREAD_STATE_EXITED	3

#ifdef	CONFIG_FIXED_STACKS
#define	THREAD_SIZE	(1 << CONFIG_FIXED_STACKS_SHIFT)
#endif

#define	TIF_NEED_RESCHED	0x00000001

#include <arch/thread.h>
#include <dlist.h>


typedef	void (*__entry_fun)(void*);

struct thread {
	struct cpu_context	cpu_context;
	__entry_fun		entry_point;
	void*			entry_data;
	unsigned int		priority;
	struct dlist		run_list;
	unsigned long		flags;
	int			state;
#ifndef	CONFIG_FIXED_STACKS
	void*			stack_base;
	unsigned long		stack_size;
};
#else
} __attribute__((aligned(THREAD_SIZE)));
#endif


static inline int thread_flag_test(const struct thread* t, unsigned long flag)
{
	return t->flags & flag;
}

static inline void thread_flag_set(struct thread* t, unsigned long flag)
{
	t->flags |= flag;
}

static inline void thread_flag_clear(struct thread* t, unsigned long flag)
{
	t->flags &= ~flag;
}


int  thread_create(struct thread* t, int priority, void (*entry_func)(void*), void* entry_data);
void thread_yield(void);

#endif
