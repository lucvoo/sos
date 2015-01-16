#ifndef	_ARCH_THREAD_H_
#define	_ARCH_THREAD_H_

struct cpu_context {
	unsigned long	r4;
	unsigned long	r5;
	unsigned long	r6;
	unsigned long	r7;
	unsigned long	r8;
	unsigned long	r9;
	unsigned long	sl;
	unsigned long	fp;
	unsigned long	sp;
	unsigned long	pc;
};

struct thread;

#ifdef	CONFIG_FIXED_STACKS
static inline struct thread* get_current_thread(void)
{
	register unsigned long sp asm ("sp");
	return (struct thread *)(sp & ~(THREAD_SIZE - 1));
}

static inline void set_current_thread(struct thread *curr)
{
}

#else

#include <stringify.h>
#include <arch/cp15.h>

static inline struct thread* get_current_thread(void)
{
	register struct thread *curr;
        asm volatile("mrc " STRINGIFY(TPIDRPRW(%0)) : "=r" (curr));
	return curr;
}

static inline void set_current_thread(struct thread *curr)
{
        asm volatile("mcr " STRINGIFY(TPIDRPRW(%0)) :: "r" (curr));
}
#endif

#endif
