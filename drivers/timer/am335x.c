#include <timer.h>
#include <io.h>
#include <interrupt.h>
#include <init.h>
#include <hw/am335x.h>
#include <hw/am335x-irqs.h>
#include <irq.h>


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


static int timer_isr(int irq, void* data)
{
	void __iomem *base = data;

	iowrite32(base + IRQSTATUS, IRQEV_OVF);

	//printf("\n%08X	timer IRQSTATUS\n", ioread32(base + IRQSTATUS));
	//printf("\n%08X	timer TCRR\n", ioread32(base + TCRR));

	return IRQ_HANDLED;
}

static int timer_dsr(int irq, unsigned int count, void* data)
{
	printf("\ntick! (count=%d)\n", count);
	return 0;
}


static struct irqaction irq_timer;

extern void am335x_intc_dump_regs(void);
static void __init timer_init(void)
{
	unsigned int ticks = 100000;
	void __iomem * base = (void*)IOBASE_DMTIMER0;
	int irq = IRQ_TINT0;

	irq_create(&irq_timer, timer_isr, timer_dsr, base, 0);
	irq_attach(&irq_timer, irq);
	timer_write(base, TLDR, 1, 0xffffffff - ticks);	// reload value
	timer_write(base, TCRR, 1, 0xffffffff - ticks);	// initial value
	timer_write(base, TCLR, 1, TCLR_ST|TCLR_AR);	// start & auto-reload
	timer_write(base, IRQENABLE_SET, 0, IRQEV_OVF);	// enable interrupts
	irq_unmask(irq);
}
subsys_initcall(timer_init);
