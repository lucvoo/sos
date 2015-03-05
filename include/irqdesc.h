#ifndef	_IRQDESC_H_
#define	_IRQDESC_H_

#include <lock.h>


struct irqaction;
struct irqchip;

struct irqdesc {
	void			(*handler)(struct irqdesc *desc);
	struct irqaction*	action;
	struct irqchip*		chip;
	unsigned int		irq;		// relative to the chip
	struct lock		lock;
};

struct irqdesc *irq_get_desc(struct irqchip *chip, unsigned int irq);


void irq_handle_level(struct irqdesc *desc);

#endif
