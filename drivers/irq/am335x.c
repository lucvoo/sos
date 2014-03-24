#include <interrupt.h>
#include <arch/irq.h>
#include <io.h>
#include <hw/am335x.h>
#include <hw/am335x-irq.h>


#ifdef DEBUG
#include "am335x-debug.c"
#endif

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
