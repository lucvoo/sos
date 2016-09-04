#include <timerdev.h>
#include <io.h>
#include <mach/hardware.h>
#include <interrupt.h>
#include <irq.h>
#include <init.h>
#include <soc/irqs.h>


// Has a 64 bit counter and 4 32 bit comparators
// The counter is counting up.
// The comparator match the lowest 32 bits of the counter

#define	TIMER_BASE	0x20003000
#define	TIMER_CS	0x00		// Control/Status
#define	TIMER_CS_M(X)	    (1 << (X))	// Match/Clear Compare bit
#define	TIMER_CLO	0x04		// Counter Low 32 bits
#define	TIMER_CHI	0x08		// Counter High 32 bits
#define	TIMER_C(X)	(0x0c + (X)*4)	// Compare 0-3

#define	TIMER_FREQ	1000000		// Wrap around after one hour
#define	TIMER_IDX	1


static int bcm2835_timer_isr(struct irqdesc *desc, void* data)
{
	struct timerdev *td = data;

	iowrite32(td->base + TIMER_CS, TIMER_CS_M(TIMER_IDX));

	return IRQ_HANDLED | IRQ_CALL_DSR;
}


static ulong bcm2835_timer_now(struct timerdev *td)
{
	ulong now = 0;

	// FIXME: 64 bits !
	now = ioread32(td->base + TIMER_CLO);
	return now;
}

static int bcm2835_timer_next_abs(struct timerdev *td, ulong val)
{
	iowrite32(td->base + TIMER_C(TIMER_IDX), val);
	return 0;
}


static struct timerdev bcm2835_timerdev = {
	.name = "bcm2835",
	.freq = TIMER_FREQ,
	.now = bcm2835_timer_now,
	.next_abs = bcm2835_timer_next_abs,
};

static struct irqaction irq_timer;

static void __init bcm2835_timer_init(void)
{
	struct irqdesc *desc;
	void __iomem *base;

	base = ioremap(TIMER_BASE, 0x1000);
	if (!base)
		return;
	bcm2835_timerdev.base = base;

	desc = irq_get_desc("intc", IRQ_TIMER);

	irq_create(&irq_timer, bcm2835_timer_isr, timerdev_dsr, &bcm2835_timerdev, 0);
	irq_attach(desc, &irq_timer);

	timerdev_register(&bcm2835_timerdev);
	irq_unmask(desc);
}
board_core_initcall(bcm2835_timer_init);
