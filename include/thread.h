#ifndef	_THREAD_H_
#define	_THREAD_H_


#define	THREAD_STATE_NEW	0
#define	THREAD_STATE_READY	1
#define	THREAD_STATE_SLEEPING	2
#define	THREAD_STATE_EXITED	3
#define	THREAD_STATE_IDLE	4

#ifdef	CONFIG_THREAD_STACK
#define	THREAD_SIZE	(1 << CONFIG_THREAD_STACK_SHIFT)
#define	__thread_align	__aligned(THREAD_SIZE)
#else
#define	__thread_align
#endif

#define	TIF_NEED_RESCHED	0x00000001

#include <arch/thread.h>
#include <dlist.h>


struct thread {
	struct cpu_context	cpu_context;
	unsigned int		priority;
	struct dlist		run_list;
	unsigned long		flags;
	int			state;
} __thread_align;


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


int  thread_create(struct thread* t, int prio, void (*func)(void*), void* data, void *stack, unsigned int size);
void thread_start(struct thread* t);
void thread_yield(void);
void thread_sleep(void);
void thread_wakeup(struct thread* t);

void thread_load_context(struct thread* t, void (*func)(void*), void* data, void* stack_top);
void __thread_start(void (*fun)(void *data), void *data);

#endif
