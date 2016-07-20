#ifndef	_ARCH_THREAD_H_
#define	_ARCH_THREAD_H_


struct cpu_context {
	// Only the call-preserved ones
	unsigned long	s[8];	// s0-s7 = $16-$23
	unsigned long	sp;	//	 = $29
	unsigned long	fp;	// s8	 = $30
	unsigned long	ra;	//	 = $31
};

struct thread;

#ifdef	CONFIG_THREAD_STACK
static inline struct thread* get_current_thread(void)
{
	register unsigned long sp asm ("sp");
	return (struct thread *)(sp & ~(THREAD_SIZE - 1));
}

static inline void set_current_thread(struct thread *curr)
{
}

#else

register struct thread *__current_thread asm("gp");
static inline struct thread* get_current_thread(void)
{
	return __current_thread;
}

static inline void set_current_thread(struct thread *new)
{
	__current_thread = new;
}
#endif

#endif
