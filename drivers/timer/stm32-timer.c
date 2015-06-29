#include <timerdev.h>
#include <io.h>
#include <interrupt.h>
#include <init.h>
#include <irq.h>
#include <arch/memory.h>
#include <soc/baseaddr.h>
#include <soc/timer.h>
#include <utils.h>
#include <errno.h>
#include <clk.h>

/*
 * For now use a single 32-bit timer in on-shot mode
 */

struct stm32_timer {
	struct timerdev		td;
	struct irqaction	irq;
	struct irqdesc		*desc;
	struct clk		*clk;
};

/**********************************************************************/
static int stm32_timer_isr(struct irqdesc * desc, void* data)
{
	struct stm32_timer *stt = data;
	struct timerdev *td = &stt->td;

	iowrite32(td->base + TIM_SR, 0);

	return IRQ_HANDLED | IRQ_CALL_DSR;
}

static int stm32_timer_dsr(struct irqdesc *desc, unsigned int count, void* data)
{
	struct stm32_timer *stt = data;
	struct timerdev *td = &stt->td;

	td->handler(td);
	return 0;
}

static unsigned long stm32_timer_now(struct timerdev *td)
{
	// FIXME 
	unsigned long reload = ioread32(td->base + TIM_ARR);
	unsigned long now    = ioread32(td->base + TIM_CNT);
	long delta = reload - now;

	return delta;
}

static int stm32_timer_next_rel(struct timerdev *td, unsigned long val)
{
	struct stm32_timer *stt = container_of(td, struct stm32_timer, td);

	// FIXME: check if val < timer's maximum

	iowrite32(td->base + TIM_ARR, val);
	iowrite32(td->base + TIM_CR1, TIM_CR1_ARPE|TIM_CR1_OPM|TIM_CR1_CEN);
	irq_unmask(stt->desc);

	return 0;
}

struct stm32_timer_cfg {
	const char*	name;
	unsigned long	base;
	int		irq;
	const char*	clk;
};

static int __init stm32_init_timer(struct stm32_timer *stt, const struct stm32_timer_cfg *cfg)
{
	void __iomem *base;
	struct irqdesc *desc;
	struct clk *clk;
	int rc = -EINVAL;


	clk = clk_get(cfg->clk);
	if (!clk)
		goto err_clk_get;
	rc = clk_enable(clk);
	if (rc)
		goto err_clk;

	rc = -EINVAL;
	base = ioremap(cfg->base, TIM_SIZE);
	if (!base)
		goto err_ioremap;

	desc = irq_get_desc("nvic", cfg->irq);
	if (!desc)
		goto err_irq;

	stt->clk = clk;
	stt->desc = desc;
	stt->td.name = cfg->name;
	stt->td.base = base;
	stt->td.next_rel = stm32_timer_next_rel;
	stt->td.now = stm32_timer_now;

	iowrite32(base + TIM_PSC, clk_get_rate(clk)/32768);
	iowrite32(base + TIM_EGR, TIM_EGR_UG);
	iowrite32(base + TIM_DIER, TIM_DIER_UIE);
	iowrite32(base + TIM_SR, 0);

	irq_create(&stt->irq, stm32_timer_isr, stm32_timer_dsr, stt, 0);
	irq_attach(desc, &stt->irq);
	// FIXME: trigger as soon as we : irq_unmask(desc);

	rc = timerdev_register(&stt->td);
	return rc;

err_irq:
	iounmap(base, TIM_SIZE);
err_ioremap:
	clk_disable(clk);
err_clk:
	clk_put(clk);

err_clk_get:
	return rc;
}


static struct stm32_timer timer2;
static const struct stm32_timer_cfg timer2_cfg = {
	.name = "timer2",
	.base = TIM2_BASE,
	.irq = TIM2_IRQ,
	.clk = "timer2clk",
};

static void __init stm32_timer_init(void)
{
	stm32_init_timer(&timer2, &timer2_cfg);
}
board_initcall(stm32_timer_init);
