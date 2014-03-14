#include <interrupt.h>
#include <arch/irq.h>
#include <io.h>
#include <hw/am335x.h>


#define INTC_REVISION		0x000
#define INTC_SYSCONFIG		0x010
#define INTC_SYSSTATUS		0x014
#define INTC_SIR_IRQ		0x040
#define INTC_SIR_FIQ		0x044
#define INTC_CONTROL		0x048
#define INTC_PROTECTION		0x04C
#define INTC_IDLE		0x050
#define INTC_IRQ_PRIORITY	0x060
#define INTC_FIQ_PRIORITY	0x064
#define INTC_THRESHOLD		0x068

#define INTC_ITR(X)		(0x080 + 0x020*(X))
#define INTC_MIR(X)		(0x084 + 0x020*(X))
#define INTC_MIR_CLEAR(X)	(0x088 + 0x020*(X))
#define INTC_MIR_SET(X)		(0x08C + 0x020*(X))
#define INTC_ISR_SET(X)		(0x090 + 0x020*(X))
#define INTC_ISR_CLEAR(X)	(0x094 + 0x020*(X))
#define INTC_PENDING_IRQ(X)	(0x098 + 0x020*(X))
#define INTC_PENDING_FIQ(X)	(0x09C + 0x020*(X))

#define	ACTIVE_IRQ_MASK		((1 << 7) -1)
#define	SPURIOUS_IRQ_MASK	(~ACTIVE_IRQ_MASK)


static void am335x_handle_irq(struct eframe *regs)
{
	void __iomem *base_addr = (void*) INTC_BASE;

	do {
		unsigned int irq;

		// Process the current interrupt
		irq = ioread32(base_addr + INTC_SIR_IRQ);
		// if (irq & SPURIOUS_IRQ_MASK)
		//	=> __do_IRQ() will treat it as spurious because it will be out-of-range
		__do_IRQ(irq, regs);

	} while (ioread32(base_addr + INTC_PENDING_IRQ(0))
	      || ioread32(base_addr + INTC_PENDING_IRQ(1))
	      || ioread32(base_addr + INTC_PENDING_IRQ(2))
	      || ioread32(base_addr + INTC_PENDING_IRQ(3))
		);
}

#include <symbols.h>
strong_alias(am335x_handle_irq, mach_irq_handler);
