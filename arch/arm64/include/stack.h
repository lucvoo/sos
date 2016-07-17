#ifndef	_ARCH_STACK_H_
#define	_ARCH_STACK_H_


#ifdef	CONFIG_THREAD_STACK

#define	__startup_stack		init_thread
#define	STARTUP_STACK_SIZE	(1 << CONFIG_THREAD_STACK_SHIFT)

#else

#define	STARTUP_STACK_SIZE	CONFIG_STARTUP_STACK_SIZE

#endif

#endif
