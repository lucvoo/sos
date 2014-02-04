#ifndef	_ARCH_EXCEPTIONS_H_
#define	_ARCH_EXCEPTIONS_H_


struct eframe {
	unsigned long	r0;
	unsigned long	r1;
	unsigned long	r2;
	unsigned long	r3;
	unsigned long	r4;
	unsigned long	r5;
	unsigned long	r6;
	unsigned long	r7;
	unsigned long	r8;
	unsigned long	r9;
	unsigned long	r10;		// sl
	unsigned long	fp;
	unsigned long	ip;
	unsigned long	sp;
	unsigned long	lr;
	unsigned long	pc;
	unsigned long	cpsr;
	unsigned long	old_r0;
};

#endif
