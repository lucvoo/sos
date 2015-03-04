/*
 * Driver for the internal 8 interrupts found in MIPS CPUs.
 * It's the root for others interrupt controller(s) that may be connected.
 *
 * The first two (IP0 & IP1) are software interrupts (needed for SMP).
 * The others are hardware interrupt sources.
 */

#include <irqchip.h>
#include <arch/regs-copro.h>
#include <arch/copro.h>
#include <arch/hazard.h>
#include <init.h>


#define	NBR_IRQS	8

static void mips_irq_mask(struct irqdesc *desc)
{
	c0_clrbits(c0_status, ST0_IM0 << desc->irq);
	ehb();
}

static void mips_irq_unmask(struct irqdesc *desc)
{
	c0_setbits(c0_status, ST0_IM0 << desc->irq);
	ehb();
}


struct mips_intctrl {
	struct irqchip	chip;
	struct irqdesc	descs[NBR_IRQS];
} mips_intctrl;

static void mips_irq_init(void)
{
	struct irqchip *chip = &mips_intctrl.chip;

	chip->name	= "mips-cpu";
	chip->irq_nbr	= NBR_IRQS;

	chip->ack	= mips_irq_mask;
	chip->mask	= mips_irq_mask;
	chip->mask_ack	= mips_irq_mask;
	chip->unmask	= mips_irq_unmask;

	irqchip_init(NULL, chip);
}
pure_initcall(mips_irq_init);

/*****************************************************************************/
#include <interrupt.h>
#include <bitops/findbit.h>

static void mips_irq_handler(struct eframe *regs, unsigned int cause, unsigned int status)
{
	unsigned int irqs = (cause & status & CAUSE_IP_MSK) >> CAUSE_IP_OFF;
	struct irqchip *chip = &mips_intctrl.chip;

	if (!irqs)
		return;			// FIXME spurious interrupt

	do {
		unsigned int irq;

		irq = bitop_fmsb(irqs);
		irqs ^= 1 << irq;
		__irq_handler(&chip->descs[irq], regs);
	} while (irqs);
}

#include <symbols.h>
strong_alias(mips_irq_handler, mach_irq_handler);
