#ifndef	_ARCH_THREAD_H_
#define	_ARCH_THREAD_H_

struct cpu_context {
	ulong	x[31];
	ulong	sp;
};

struct thread;


#ifdef	CONFIG_FIXED_STACKS
static inline ulong * get_current_sp(void)
{
	register ulong *sp asm ("sp");
	return sp;
}

static inline struct thread* get_current_thread(void)
{
	register ulong sp asm ("sp");
	return (struct thread *)(sp & ~(THREAD_SIZE - 1));
}

static inline void set_current_thread(struct thread *curr)
{
}
#endif

#endif
