#ifndef	_ARCH_EXCEPTIONS_H_
#define	_ARCH_EXCEPTIONS_H_


struct eframe {

	unsigned long	r[13];
	unsigned long	lr;
	unsigned long	sp;
	unsigned long	pc;
	unsigned long	cpsr;
#if CONFIG_ARM_ARCH < 6
	unsigned long	old_r0;
#endif
};


// called from asm
void pabt_handler(struct eframe *regs, ulong far, ulong fsr);
void dabt_handler(struct eframe *regs, ulong far, ulong fsr);

#endif
