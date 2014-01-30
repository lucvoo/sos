#ifndef	_ARCH_MACH_IRQ_H_
#define	_ARCH_MACH_IRQ_H_


#define	NR_IRQS	128


#ifndef	__ASSEMBLY__
static inline void irq_mask(unsigned int irq)
{
	/* FIXME */
}

static inline void irq_mask_ack(unsigned int irq)
{
	/* FIXME */
}

static inline void irq_unmask(unsigned int irq)
{
	/* FIXME */
}
#endif

#endif
