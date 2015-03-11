#include <smp/init.h>
#include <arch/copro.h>
#include <arch/memory.h>
#include <io.h>
#include <soc/cgu.h>
#include <soc/regs-copro.h>
#include <thread.h>
#include <smp.h>


/******************************************************************************/
/******** IPI ********/

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
 */
static int mbox_irq_handler(struct irqdesc *desc, void *data)
{
	unsigned int msg;

	lock_acq(&core_regs_lock);
	switch (__coreid()) {
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

	__smp_ipi_process(msg);

	return IRQ_HANDLED;
}

static struct irqaction mbox_action;
/******************************************************************************/


static struct thread idle_thread_core1;

struct thread *__smp_sec_cpu_thread;

void __smp_init(void)
{
	extern void __jz4780_smp_sec_cpu_entry(void);

	void __iomem *cgu = ioremap(CGU_BASE, CGU_SIZE);
	unsigned long entry;
	unsigned int gated;
	unsigned int lpcr;

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

	// initialize the thread with the default stack size
	thread_create(&idle_thread_core1, 0, NULL, NULL, NULL, 0);

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

	// setup the address of the entry point, its setup stack and un-reset the core
	entry = (unsigned long) &__jz4780_smp_sec_cpu_entry;
	__smp_sec_cpu_thread = &idle_thread_core1;
	c0_chgbits(c0_core_reim, CORE_REIM_ENTRY_MSK, entry - KSEG0_BASE + KSEG1_BASE);
	c0_chgbits(c0_core_ctrl, CORE_CTRL_SW_RST1|CORE_CTRL_RPC1, CORE_CTRL_RPC1);
}
