#ifndef	_INTERRUPT_H_
#define	_INTERRUPT_H_


struct eframe;

void __do_IRQ(unsigned int irq, struct eframe *regs);
void __do_softirq(void);


typedef int (*isr_handler_t)(int, void *);
typedef int (*dsr_handler_t)(int, unsigned int, void *);

// Return values for isr & dsr handlers
#define IRQ_NONE	0	// We didn't handled it
#define IRQ_HANDLED	1	// It was a valid interrupt and we handled it
#define IRQ_CALL_DSR	2

struct irqaction {
	isr_handler_t	isr_handler;
#ifdef	CONFIG_DSR
	dsr_handler_t	dsr_handler;
	unsigned long	dsr_count;
#endif
	unsigned long	flags;
	void*		data;
};

int irq_attach(struct irqaction* action, int irq);
void irq_create(struct irqaction* action, isr_handler_t isr_handler, dsr_handler_t dsr_handler, void* data, unsigned long flags);

#endif
