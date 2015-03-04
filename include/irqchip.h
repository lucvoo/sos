#ifndef	_IRQCHIP_H_
#define	_IRQCHIP_H_

#include <iomem.h>

struct irqdesc;

struct irqchip {
	const char		*name;
	void __iomem		*iobase;
	void			(*ack)(struct irqdesc *desc);
	void			(*mask)(struct irqdesc *desc);
	void			(*mask_ack)(struct irqdesc *desc);
	void			(*unmask)(struct irqdesc *desc);

	unsigned short		irq_nbr;
	struct irqdesc		*descs;
};

void irqchip_init(struct irqdesc *parent, struct irqchip *chip);

#endif
