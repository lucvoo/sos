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

#endif
