#ifndef	_INTERRUPT_H_
#define	_INTERRUPT_H_


struct eframe;

void __do_IRQ(unsigned int irq, struct eframe *regs);


typedef int (*isr_handler_t)(int, void *);
typedef int (*dsr_handler_t)(int, unsigned int, void *);

// Return values for isr & dsr handlers
#define IRQ_NONE	0	// We didn't handled it
#define IRQ_HANDLED	1	// It was a valid interrupt and we handled it

struct irqaction {
	isr_handler_t	isr_handler;
	dsr_handler_t	dsr_handler;
	unsigned long	flags;
	void*		data;
};

#endif
