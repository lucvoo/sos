#include <timerdev.h>
#include <io.h>
#include <interrupt.h>
#include <init.h>
#include <hw/am335x.h>
#include <hw/am335x-irqs.h>
#include <irq.h>
#include <utils.h>


#define	TIDR		0x00
#define	TIOCP_CFG	0x10
#define	IRQ_EOI		0x20
#define	IRQSTATUS_RAW	0x24
#define	IRQSTATUS	0x28
#define	IRQENABLE_SET	0x2C
#define	IRQENABLE_CLR	0x30
#define	IRQWAKEEN	0x34
#define		IRQEV_MAT	0x1
#define		IRQEV_OVF	0x2
#define		IRQEV_CAP	0x4
#define	TCLR		0x38
#define		TCLR_ST		0x01
#define		TCLR_AR		0x02
#define		TCLR_CE		0x40
#define	TCRR		0x3C
#define	TLDR		0x40
#define	TTGR		0x44
#define	TWPS		0x48
#define	TMAR		0x4C
#define	TCAR1		0x50
#define	TSICR		0x54
#define	TCAR2		0x58

#define	WPSBIT(R)	(1 << ((R - TCLR)/4))


static void timer_wps(void __iomem *base, unsigned int reg)
{
	while (ioread32(base + TWPS) & WPSBIT(reg))
		;
}

static unsigned int timer_read(void __iomem *base, unsigned int reg, int posted)
{
	if (posted)
		timer_wps(base, reg);

	return ioread32(base + reg);
}

static void timer_write(void __iomem *base, unsigned int reg, int posted, unsigned int val)
{
	if (posted)
		timer_wps(base, reg);

	iowrite32(base + reg, val);
}


static int timer_isr(struct irqdesc *desc, void* data)
{
	struct timerdev *td = data;

	iowrite32(td->base + IRQSTATUS, IRQEV_MAT);

	//printf("\n%08X	timer IRQSTATUS\n", ioread32(base + IRQSTATUS));
	//printf("\n%08X	timer TCRR\n", ioread32(base + TCRR));

	return IRQ_HANDLED | IRQ_CALL_DSR;
}


static unsigned long timer_am335x_now(struct timerdev *td)
{
	unsigned long now = timer_read(td->base, TCRR, 1);

	return now;
}

static int timer_am335x_next_abs(struct timerdev *td, unsigned long val)
{
	long delta = timer_am335x_now(td) - val;

	if (delta >= 0)
		return -1;

	timer_write(td->base, TMAR, 1, val);		// match register
	return 0;
}

static struct timerdev timerdev_am335x = { 
	.name = "am335x",
	.freq = 32*1024,
	.now = timer_am335x_now,
	.next_abs = timer_am335x_next_abs,
};


static struct irqaction irq_timer;

static void __init timer_init(void)
{
	struct irqdesc *desc;
	void __iomem *base;

	base = ioremap(IOBASE_DMTIMER0, 0x1000);
	if (!base)
		return;

	desc = irq_get_desc("intc", IRQ_TINT0);

	timerdev_am335x.base = base;

	irq_create(&irq_timer, timer_isr, timerdev_dsr, &timerdev_am335x, 0);
	irq_attach(desc, &irq_timer);
	timer_write(base, TMAR, 1, -1);			// match register: a full cycle by default
	timer_write(base, TLDR, 1, 0);			// reload value
	timer_write(base, TTGR, 1, 1);			// trigger reload
	timer_write(base, TCLR, 1, TCLR_ST|TCLR_AR|TCLR_CE);	// start, auto-reload & compare
	timer_write(base, IRQENABLE_SET, 0, IRQEV_MAT);	// enable interrupts
	irq_unmask(desc);

	timerdev_register(&timerdev_am335x);
}
board_core_initcall(timer_init);
