#ifndef	_ARCH_EXCEPTIONS_H_
#define	_ARCH_EXCEPTIONS_H_


struct eframe {

	unsigned long	r[13];
	unsigned long	sp;
	unsigned long	lr;
	unsigned long	pc;
	unsigned long	cpsr;
#if CONFIG_ARM_ARCH < 6
	unsigned long	old_r0;
#endif
};

#endif
