#ifndef	_ARCH_IRQ_H_
#define	_ARCH_IRQ_H_


#warning "Need to be implemented"

#define	NR_IRQS	(8+64)			// FIXME: this is SOC dependent


static inline void irq_ack(unsigned int irq)
{
}

static inline void irq_mask(unsigned int irq)
{
}

static inline void irq_mask_ack(unsigned int irq)
{
}

static inline void irq_unmask(unsigned int irq)
{
}

#endif
