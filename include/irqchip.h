#ifndef	_IRQCHIP_H_
#define	_IRQCHIP_H_

#include <iomem.h>


struct irqchip {
	const char		*name;
	void __iomem		*iobase;
	void			(*ack)(int irq);
	void			(*mask)(int irq);
	void			(*mask_ack)(int irq);
	void			(*unmask)(int irq);

	unsigned short		irq_nbr;
};

extern struct irqchip mach_irqchip;

#endif
