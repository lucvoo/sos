#include <smp/init.h>
#include <smp/ops.h>
#include <arch/copro.h>
#include <arch/memory.h>
#include <io.h>
#include <soc/cgu.h>
#include <soc/regs-copro.h>
#include <thread.h>
#include <smp.h>


/******************************************************************************/
/******** IPI ********/

#include <bitops/findbit.h>
#include <arch/hazard.h>
#include <interrupt.h>
#include <irqdesc.h>
#include <lock.h>
#include <smp/ipi.h>
#include <sched.h>

#define	IRQ_MBOX		3 // on MIPS internal irqs

/* This lock protect the 'core' registers:
 *	- c0_core_ctrl
 *	- c0_core_stat
 *	- c0_core_reim
 *	- c0_mailbox[01]
 */
static struct lock core_regs_lock;

/* The JZ4780's mailboxes are for sending short messages (a single word)
 * between the cores.
 * They are (used to implement) the Inter-Process Interrupts.
 *
 * Here we convert IPIs number into bitfields forth & back
 * since it seems to be normal thing to do with those mailboxes
 * (it guarantee us that IPIs are never lost).
 */
static int mbox_irq_handler(struct irqdesc *desc, void *data)
{
	unsigned int msg;

	lock_acq(&core_regs_lock);
	switch (__cpuid()) {
	case 0:
		msg = c0_getval(c0_mailbox0);
		c0_setval(c0_mailbox0, 0);
		c0_clrbits(c0_core_stat, 1 << 0);
		break;
	case 1:
		msg = c0_getval(c0_mailbox1);
		c0_setval(c0_mailbox1, 0);
		c0_clrbits(c0_core_stat, 1 << 1);
		break;
	default:
		// FIXME
		msg = 0;
		break;
	}
	lock_rel(&core_regs_lock);

	while (msg) {
		uint ipi = bitop_clz(msg);

		msg &= ~(1 << ipi);
		__smp_ipi_process(ipi);
	}

	return IRQ_HANDLED;
}

void __smp_ipi_send(uint cpu, uint ipi)
{
	lock_acq(&core_regs_lock);
	switch (cpu) {
	case 0:
		c0_setbits(c0_mailbox0, 1 << ipi);
		break;
	case 1:
		c0_setbits(c0_mailbox1, 1 << ipi);
		break;
	default:
		// FIXME
		break;
	}
	lock_rel(&core_regs_lock);
}

static struct irqaction mbox_action;

/******************************************************************************/

static int jz4780_smp_init(void)
{
	// setup the mailboxes' irq handler
	irq_create(&mbox_action, mbox_irq_handler, NULL, NULL, 0);
	irq_attach(irq_get_desc("cpuintc", IRQ_MBOX), &mbox_action);

	c0_clrbits(c0_core_reim, 0x1);
	c0_setval(c0_mailbox0, 0x0);
	c0_setval(c0_mailbox1, 0x0);
	c0_setval(c0_core_stat, 0x0);
	c0_setbits(c0_core_reim, 0x3);
	c0_setbits(c0_status, ST0_IM0 << IRQ_MBOX);
	ehb();

	return 0;
}

static int jz4780_smp_boot_cpu(struct thread *idle, uint cpu)
{
	extern void __smp_entry(void);

	void __iomem *cgu = ioremap(CGU_BASE, CGU_SIZE);
	unsigned long entry;
	unsigned int gated;
	unsigned int lpcr;

	// ungate the core clock
	gated = ioread32(cgu + CGU_CLKGR1);
	gated &= ~CGU_CLKGR1_CORE1;
	iowrite32(cgu + CGU_CLKGR1, gated);

	// powerup the core
	lpcr = ioread32(cgu + CGU_LPCR);
	lpcr &= ~CGU_LPCR_SCPU_PD;
	iowrite32(cgu + CGU_LPCR, lpcr);

	// wait until powered
	while(ioread32(cgu + CGU_LPCR) & CGU_LPCR_SCPU_ST)
		;

	// setup the address of the entry point
	entry = (unsigned long) &__smp_entry;
	c0_chgbits(c0_core_reim, CORE_REIM_ENTRY_MSK, entry - KSEG0_BASE + KSEG1_BASE);

	// un-reset the core
	c0_chgbits(c0_core_ctrl, CORE_CTRL_SW_RST1|CORE_CTRL_RPC1, CORE_CTRL_RPC1);

	return 0;
}


static int jz4780_smp_init_cpu(uint cpu)
{
	return 0;
}

struct smp_ops smp_ops = {
	.init     = jz4780_smp_init,
	.init_cpu = jz4780_smp_init_cpu,
	.boot_cpu = jz4780_smp_boot_cpu,
};
