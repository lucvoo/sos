#ifndef	_ARCH_IRQ_H_
#define	_ARCH_IRQ_H_

#include <mach/irq.h>

struct eframe;
void mach_irq_handler(struct eframe *regs);

#endif
