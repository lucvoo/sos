#ifndef	_ARCH_EXCEPTIONS_H_
#define	_ARCH_EXCEPTIONS_H_


struct eframe {
	unsigned long	x[31];
	unsigned long	sp;
	unsigned long	pc;
	unsigned long	pstate;
};

#endif
