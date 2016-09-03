/*
 *  Atheros AR71xx/AR724x/AR913x specific interrupt handling
 *
 *  Copyright (C) 2010-2011 Jaiganesh Narayanan <jnarayanan@atheros.com>
 *  Copyright (C) 2008-2011 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  Parts of this file are based on Atheros' 2.6.15/2.6.31 BSP
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <irqchip.h>
#include <soc/ar71xx_regs.h>
#include <soc/irqs.h>
#include <bitops/findbit.h>
#include <init.h>
#include <io.h>


#define	NBR_IRQS	17

static struct ath79_misc_irqchip {
	struct irqchip	chip;
	struct irqdesc	descs[NBR_IRQS];
} ath79_misc_irqchip;


static void ar71xx_irq_misc_ack(struct irqdesc *desc)
{
	void __iomem *iobase = desc->chip->iobase + AR71XX_RESET_REG_MISC_INT_STATUS;
	uint irq = desc->irq;
	u32 msk;

	msk = ioread32(iobase);
	iowrite32(iobase, msk & ~(1 << irq));

	// flush early write's ack
	ioread32(iobase);
}

static void ar71xx_irq_misc_mask(struct irqdesc *desc)
{
	void __iomem *iobase = desc->chip->iobase + AR71XX_RESET_REG_MISC_INT_ENABLE;
	uint irq = desc->irq;
	u32 msk;

	msk = ioread32(iobase);
	iowrite32(iobase, msk & ~(1 << irq));

	// flush early write's ack
	ioread32(iobase);
}

static void ar71xx_irq_misc_unmask(struct irqdesc *desc)
{
	void __iomem *iobase = desc->chip->iobase + AR71XX_RESET_REG_MISC_INT_ENABLE;
	uint irq = desc->irq;
	u32 msk;

	msk = ioread32(iobase);
	iowrite32(iobase, msk | (1 << irq));

	// flush early write's ack
	ioread32(iobase);
}


static void ath79_misc_irq_handler(struct irqdesc *desc)
{
	struct irqchip *chip = &ath79_misc_irqchip.chip;
	void __iomem *iobase = chip->iobase;
	u32 irqs;

	irqs = ioread32(iobase + AR71XX_RESET_REG_MISC_INT_STATUS) &
	       ioread32(iobase + AR71XX_RESET_REG_MISC_INT_ENABLE);

	if (!irqs) {
		// FIXME: irq_spurious(desc);
		return;
	}

	do {
		uint irq = bitop_clz(irqs);

		irq_handle_desc(&chip->descs[irq]);
		irqs &= ~(1 << irq);
	} while (irqs);
}


static void __init ath79_misc_irq_init(void)
{
	struct irqchip *chip = &ath79_misc_irqchip.chip;
	struct irqdesc *parent;
	void __iomem *iobase;


	parent = irq_get_desc("cpuintc", IRQ_CPU_MISC);

	iobase	= ioremap(AR71XX_RESET_BASE, AR71XX_RESET_SIZE);

	chip->name	= "misc",
	chip->irq_nbr	= NBR_IRQS;
	chip->iobase	= iobase;

	chip->unmask	= ar71xx_irq_misc_unmask,
	chip->mask	= ar71xx_irq_misc_mask,
	chip->ack	= ar71xx_irq_misc_ack,

	// mask & ack all interrupts
	iowrite32(iobase + AR71XX_RESET_REG_MISC_INT_ENABLE, 0);
	iowrite32(iobase + AR71XX_RESET_REG_MISC_INT_STATUS, 0);

	irqchip_chain(parent, ath79_misc_irq_handler, chip);
}
board_irq_initcall(ath79_misc_irq_init);
