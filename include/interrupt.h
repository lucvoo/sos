#ifndef	_INTERRUPT_H_
#define	_INTERRUPT_H_

struct eframe;

void __do_IRQ(unsigned int irq, struct eframe *regs);

#endif
