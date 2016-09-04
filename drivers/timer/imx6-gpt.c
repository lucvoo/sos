#include <timerdev.h>
#include <interrupt.h>
#include <errno.h>
#include <init.h>
#include <irq.h>
#include <clk.h>
#include <io.h>


#define	GPT_IRQ		87
#define	GPT_BASE	0x02098000
#define	GPT_SIZE	    0x1000

#define	GPT_CR		0x00
#define	GPT_CR_EN		(1 << 0)
#define	GPT_CR_ENMOD		(1 << 1)
#define	GPT_CR_DBGEN		(1 << 2)
#define	GPT_CR_WAITEN		(1 << 3)
#define	GPT_CR_DOZEEN		(1 << 4)
#define	GPT_CR_STOPEN		(1 << 5)
#define	GPT_CR_CLKSRC_NO	(0 << 6)
#define	GPT_CR_CLKSRC_PERIPH	(1 << 6)
#define	GPT_CR_CLKSRC_HFREF	(2 << 6)
#define	GPT_CR_CLKSRC_CLKIN	(3 << 6)
#define	GPT_CR_CLKSRC_LFREF	(4 << 6)
#define	GPT_CR_CLKSRC_CRYS	(5 << 6)
#define	GPT_CR_FRR		(1 << 9)
#define	GPT_CR_EN_24M		(1 << 10)
#define	GPT_CR_SWR		(1 << 15)
#define	GPT_CR_OM1(M)		(M << 20)
#define	GPT_CR_OM2(M)		(M << 23)
#define	GPT_CR_OM3(M)		(M << 26)
#define	GPT_CR_OM_NOPE		0
#define	GPT_CR_OM_TOGGLE	1
#define	GPT_CR_OM_CLEAR		2
#define	GPT_CR_OM_SET		3
#define	GPT_CR_OM_PULSE		4
#define	GPT_CR_FO1		(1 << 29)
#define	GPT_CR_FO2		(1 << 30)
#define	GPT_CR_FO3		(1 << 31)

#define	GPT_PR		0x04
#define	GPT_PR_PRESCALER(S)	(((S) -1) << 0)
#define	GPT_PR_PRESCALER_24M(S)	(((S) -1) << 12)

#define	GPT_SR		0x08
#define	GPT_SR_OF1		(1 << 0)
#define	GPT_SR_OF2		(1 << 1)
#define	GPT_SR_OF3		(1 << 2)
#define	GPT_SR_IF1		(1 << 3)
#define	GPT_SR_IF2		(1 << 4)
#define	GPT_SR_ROV		(1 << 5)

#define	GPT_IR		0x0C
#define	GPT_IR_OF1		(1 << 0)
#define	GPT_IR_OF2		(1 << 1)
#define	GPT_IR_OF3		(1 << 2)
#define	GPT_IR_IF1		(1 << 3)
#define	GPT_IR_IF2		(1 << 4)
#define	GPT_IR_ROV		(1 << 5)

#define	GPT_OCR1	0x10
#define	GPT_OCR2	0x14
#define	GPT_OCR3	0x18
#define	GPT_ICR1	0x1C
#define	GPT_ICR2	0x20
#define	GPT_CNT		0x24


/**********************************************************************/
/*
 * Operations:
 * - roll-over interrupts are always enabled
 * - match interrupt are enbled only when needed
 */

static int gpt_isr(struct irqdesc *desc, void *data)
{
	struct timerdev *td = data;
	u32 status;

	status = ioread32(td->base + GPT_SR);
	status &= GPT_SR_OF1 | GPT_SR_ROV;
	if (!status)
		return IRQ_NONE;

	iowrite32(td->base + GPT_IR, GPT_IR_ROV);
	iowrite32(td->base + GPT_SR, status);
	return IRQ_HANDLED | IRQ_CALL_DSR;
}

static ulong gpt_now(struct timerdev *td)
{
	ulong now;

	now = ioread32(td->base + GPT_CNT);
	return now;
}

static int gpt_next_abs(struct timerdev *td, ulong val)
{
	iowrite32(td->base + GPT_OCR1, val);
	iowrite32(td->base + GPT_IR, GPT_IR_ROV|GPT_IR_OF1);
	return 0;
}

static int gpt_next_rel(struct timerdev *td, ulong val)
{
	ulong now = gpt_now(td);

	return gpt_next_abs(td, now + val);
}

static struct timerdev gpt;
static struct irqaction gpt_irq;

static int __init gpt_init_timer(void)
{
	struct irqdesc *desc;
	struct clk *clk;
	void __iomem *base;
	int rc = -EINVAL;
	ulong rate;

	base = ioremap(GPT_BASE, GPT_SIZE);
	if (!base)
		goto err_ioremap;

	desc = irq_get_desc("gic", GPT_IRQ);
	if (!desc)
		goto err_irq;

	clk = clk_get("ckil");
	if (!clk)
		goto err_clk;
	clk_enable(clk);
	rate = clk_get_rate(clk);

	gpt.base = base;
	gpt.name = "gpt",
	gpt.now = gpt_now,
	gpt.next_abs = gpt_next_abs,
	gpt.next_rel = gpt_next_rel,

	iowrite32(base + GPT_CR, 0);
	iowrite32(base + GPT_OCR1, ~0);
	iowrite32(base + GPT_IR, GPT_IR_ROV|GPT_IR_OF1);
	iowrite32(base + GPT_PR, GPT_PR_PRESCALER(rate/HZ));
	iowrite32(base + GPT_CR, GPT_CR_FRR|GPT_CR_WAITEN|GPT_CR_EN |GPT_CR_CLKSRC_LFREF);

	irq_create(&gpt_irq, gpt_isr, timerdev_dsr, &gpt, 0);
	irq_attach(desc, &gpt_irq);
	irq_unmask(desc);

	rc = timerdev_register(&gpt);
	if (rc == 0)
		return rc;

	clk_put(clk);
err_clk:
	// FIXME: irq_put_desc() ?
err_irq:
	iounmap(base, GPT_SIZE);
err_ioremap:
	return rc;
}

static void __init gpt_init(void)
{
	gpt_init_timer();
}
board_initcall(gpt_init);
