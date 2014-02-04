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
#endif

#define	thread_initial_context(t, func, data)	\
do {						\
	func = (void*) t->cpu_context.r4;	\
	data = (void*) t->cpu_context.r5;	\
} while (0)

#endif
