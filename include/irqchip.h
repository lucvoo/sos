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

extern struct irqchip mach_irqchip;

#endif
