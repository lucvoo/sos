#include <timerdev.h>
#include <io.h>
#include <interrupt.h>
#include <init.h>
#include <soc/irqs.h>
#include <irq.h>


#define	TIMERX_IRQ(T)	(TIMER0_IRQ + (T))

#define	TIMER_BASE	0x06000C00
#define	TIMER_SIZE	0x000000A0


#define TIMER_IRQ_EN		0x00
#define TIMER_IRQ_STATUS	0x04
#define TIMER_IRQ(T)			(1 << (T))

#define TIMER_CTRL(T)		(0x10 + (T) * 0x10)
#define CTRL_ONESHOT			(1 << 7)
#define CTRL_PRESCALE(DIV)		(((DIV) & 0x7) << 4)
#define CTRL_OSC32K			(0 << 2)
#define CTRL_OSC24M			(1 << 2)
#define CTRL_RELOAD			(1 << 1)
#define CTRL_ENABLE			(1 << 0)

#define TIMER_INTV(T)		(0x14 + (T) * 0x10)
#define TIMER_CURR(T)		(0x18 + (T) * 0x10)


#define	TIMER_FREERUNNING	0
#define	TIMER_ONESHOT		1


static int timer_isr(struct irqdesc *desc, void* data)
{
	struct timerdev *td = data;

	iowrite32(td->base + TIMER_IRQ_STATUS, TIMER_IRQ(TIMER_ONESHOT));

	return IRQ_HANDLED | IRQ_CALL_DSR;
}


static void sunxi_timer_stop(struct timerdev *td)
{
	u32 ctrl = CTRL_OSC32K|CTRL_ONESHOT;

	iowrite32(td->base + TIMER_CTRL(TIMER_ONESHOT), ctrl);

	// FIXME:wait at least 2 cycles
}

static int sunxi_timer_next_rel(struct timerdev *td, ulong val)
{
	u32 ctrl = CTRL_OSC32K|CTRL_ONESHOT|CTRL_ENABLE|CTRL_RELOAD;

	iowrite32(td->base + TIMER_INTV(TIMER_ONESHOT), val);
	iowrite32(td->base + TIMER_CTRL(TIMER_ONESHOT), ctrl);
	return 0;
}

static ulong sunxi_timer_now(struct timerdev *td)
{
	ulong now = ioread32(td->base + TIMER_CURR(TIMER_FREERUNNING));

	now = 0xffffffff - now;
	return now;
}

static struct timerdev sunxi_timerdev = {
	.name = "sunxi",
	.freq = 32 * 1024,
	.now = sunxi_timer_now,
	.next_rel = sunxi_timer_next_rel,
};


static struct irqaction irq_timer;

static void __init sunxi_timer_init(void)
{
	struct timerdev *td = &sunxi_timerdev;
	struct irqdesc *desc;
	void __iomem *base;
	u32 ctrl;

	base = ioremap(TIMER_BASE, 0xA0);
	if (!base)
		return;

	desc = irq_get_desc("gic", TIMERX_IRQ(TIMER_ONESHOT));

	td->base = base;

	// setup the free running counter
	iowrite32(td->base + TIMER_CURR(TIMER_FREERUNNING),  0x0);
	iowrite32(td->base + TIMER_INTV(TIMER_FREERUNNING), ~0x0);
	ctrl = CTRL_OSC32K|CTRL_ENABLE|CTRL_RELOAD;
	iowrite32(td->base + TIMER_CTRL(TIMER_FREERUNNING), ctrl);

	// setup the (real) timer
	irq_create(&irq_timer, timer_isr, timerdev_dsr, &sunxi_timerdev, 0);
	irq_attach(desc, &irq_timer);

	ctrl = CTRL_OSC32K|CTRL_ONESHOT;
	iowrite32(td->base + TIMER_CTRL(TIMER_ONESHOT), ctrl);
	iowrite32(td->base + TIMER_IRQ_EN, TIMER_IRQ(TIMER_ONESHOT));

	irq_unmask(desc);

	timerdev_register(td);
}
board_core_initcall(sunxi_timer_init);
