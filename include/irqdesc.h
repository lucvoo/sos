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

struct irqdesc *irq_get_desc(const char *name, unsigned int irq);


void irq_handle_level(struct irqdesc *desc);
void irq_handle_simple(struct irqdesc *desc);

void irq_handle_desc(struct irqdesc *desc);	// small helper calling its own handler

#endif
