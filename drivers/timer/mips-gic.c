#include <timerdev.h>
#include <interrupt.h>
#include <irqdesc.h>
#include <arch/gic.h>
#include <arch/cm.h>
#include <types.h>
#include <errno.h>
#include <io.h>


#if BITS_PER_LONG != 32
#error "64bit GIC is not supported yet"
#endif

// FIXME: We scale down all the counter related values
// because it's way too fast (until we have proper 64bit support)
#define	COUNTER_SHIFT	7

static u64 gic_timer_get_cnt(void __iomem *base)
{
	u64 val;

	base += GIC_SH_OFFSET;
	val = ioread2x32(base + GIC_SH_CNTHi, base + GIC_SH_CNTLo);
	return val;
}

static u64 gic_timer_get_cmp(void __iomem *base)
{
	base += GIC_VP_OFFSET;
	return ioread2x32(base + GIC_VP_CMPHi, base + GIC_VP_CMPLo);
}

static void gic_timer_set_cmp(void __iomem *base, u64 cmp)
{
	base += GIC_VP_OFFSET;
	iowrite2x32(base + GIC_VP_CMPHi, base + GIC_VP_CMPLo, cmp);
}


static int gic_timer_isr(struct irqdesc *desc, void *data)
{
	struct timerdev *td = data;
	void __iomem *base = td->base;

	// ack the interrupt by writting to the register
	gic_timer_set_cmp(base, 0);

	return IRQ_HANDLED | IRQ_CALL_DSR;
}

// FIXME: - 32bit but HW allow 64bit
//	    => fast rollover
static ulong gic_timer_now(struct timerdev *td)
{
	u64 now;

	now = gic_timer_get_cnt(td->base);
	return now >> COUNTER_SHIFT;
}

static int gic_timer_next_rel(struct timerdev *td, ulong val)
{
	void __iomem *base = td->base;
	u64 now = gic_timer_get_cnt(base);
	u64 exp = val;

	gic_timer_set_cmp(base, now + (exp << COUNTER_SHIFT));
	return 0;
}


static void gic_timer_start_cnt(void __iomem *base)
{
	base += GIC_SH_OFFSET;
	ioclr32(base + GIC_SH_CFG, GIC_SH_CFG_COUNTSTOP);
}

static void gic_timer_stop_cnt(void __iomem *base)
{
	base += GIC_SH_OFFSET;
	ioset32(base + GIC_SH_CFG, GIC_SH_CFG_COUNTSTOP);
}

static uint gic_timer_get_countbits(void __iomem *base)
{
	u32 reg;

	base += GIC_SH_OFFSET;
	reg = ioread32(base + GIC_SH_CFG);
	return GIC_SH_CFG_COUNTBITS(reg);
}


/******************************************************************************/
#include <arch/gic-irqs.h>
#include <irq.h>

static struct irqdesc* gic_timer_get_irqdesc(void)
{
	return irq_get_desc("mips-gic-vp", GIC_COMP);
}

static int gic_timer_init_local(struct timerdev *td, uint cpu)
{
	struct irqdesc *desc = gic_timer_get_irqdesc();

	irq_unmask(desc);
	return 0;
}


#include <init.h>
static int __init gic_timerdev_init(struct timerdev *td)
{
	static struct irqaction gic_timer_irq;
	struct irqdesc *desc;
	int rc = EINVAL;

	desc = gic_timer_get_irqdesc();
	if (!desc)
		goto err_irqdesc;

	td->name = "gic-cmp";
	td->base = ioremap(GIC_BASE, GIC_SIZE);
	td->now      = gic_timer_now;
	td->next_rel = gic_timer_next_rel;

	irq_create(&gic_timer_irq, gic_timer_isr, timerdev_dsr, td, 0);
	irq_attach(desc, &gic_timer_irq);

	gic_timer_init_local(td, 0);

	rc = timerdev_register(td);
	return rc;

err_irqdesc:
	return rc;
}

static struct timerdev gic_timerdev;
static void __init gic_timer_init(void)
{
	gic_timerdev_init(&gic_timerdev);
}
board_initcall(gic_timer_init);

#include <smp/initcall.h>
static void gic_timer_init_smp(uint cpu)
{
	gic_timer_init_local(&gic_timerdev, cpu);
}
core_smp_initcall(gic_timer_init_smp);
