#include <irqchip.h>
#include <irqdesc.h>
#include <io.h>
#include <hw/am335x.h>
#include <hw/am335x-irq.h>
#include <init.h>


#ifdef DEBUG
#include "am335x-debug.c"
#endif

#define	INTC_NBR_IRQ	128


static inline void am33xx_irq_ack(struct irqdesc *desc)
{
	void __iomem *iobase = mach_irqchip.iobase;

	iowrite32(iobase + INTC_CONTROL, 1);
}

static inline void am33xx_irq_mask(struct irqdesc *desc)
{
	void __iomem *iobase = mach_irqchip.iobase;
	unsigned int irq = desc->irq;

	iowrite32(iobase + INTC_MIR_SET(irq / 32), 1 << (irq  % 32));
}

static inline void am33xx_irq_unmask(struct irqdesc *desc)
{
	void __iomem *iobase = mach_irqchip.iobase;
	unsigned int irq = desc->irq;

	iowrite32(iobase + INTC_MIR_CLEAR(irq / 32), 1 << (irq  % 32));
}


static struct irqdesc	irqdescs[INTC_NBR_IRQ];
struct irqchip	mach_irqchip;

static void am335x_irq_init(void)
{
	void __iomem *iobase = ioremap(INTC_BASE, 0x1000);
	struct irqchip *chip = &mach_irqchip;

	// FIXME: should be dynamically allocated
	chip->iobase	= iobase;
	chip->name	= "am33xx-intc";
	chip->irq_nbr	= INTC_NBR_IRQ;
	chip->descs	= irqdescs;

	chip->ack	= am33xx_irq_ack;
	chip->mask	= am33xx_irq_mask;
	chip->unmask	= am33xx_irq_unmask;
}
board_irq_initcall(am335x_irq_init);

/*****************************************************************************/
#include <interrupt.h>
struct eframe;

static void am335x_handle_irq(struct eframe *regs)
{
	void __iomem *base_addr = mach_irqchip.iobase;

	do {
		struct irqdesc *desc;
		unsigned int irq;

		// Process the current interrupt
		irq = ioread32(base_addr + INTC_SIR_IRQ);
		// if (irq & SPURIOUS_IRQ_MASK)
		//	=> __do_IRQ() will treat it as spurious because it will be out-of-range

		desc = irq_get_desc(NULL, irq);
		__irq_handler(desc, regs);

	} while (ioread32(base_addr + INTC_PENDING_IRQ(0))
	      || ioread32(base_addr + INTC_PENDING_IRQ(1))
	      || ioread32(base_addr + INTC_PENDING_IRQ(2))
	      || ioread32(base_addr + INTC_PENDING_IRQ(3))
		);
}

#include <symbols.h>
strong_alias(am335x_handle_irq, mach_irq_handler);
