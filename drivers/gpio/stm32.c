#include "gpiochip.h"
#include <irqchip.h>
#include <soc/rcc.h>
#include <soc/gpio.h>
#include <soc/baseaddr.h>
#include <io.h>
#include <init.h>


static void stm32_gpio_input(struct gpio *gpio)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n = gpio->idx;

	ioclrset32(iobase + GPIO_MODER, GPIO_MODE_MASK(n), GPIO_MODE_IN(n), 0);
}

static int  stm32_gpio_get_value(struct gpio *gpio)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n = gpio->idx;

	return (ioread32(iobase + GPIO_IDR) >> n) & 0x1;
}

static void stm32_gpio_set_value(struct gpio *gpio, int val)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n =  gpio->idx;
	u32 bit;

	if (val)
		bit = GPIO_BSET(n);
	else
		bit = GPIO_BCLR(n);

	iowrite32(iobase + GPIO_BSRR, bit);
}

static void stm32_gpio_output(struct gpio *gpio, int val)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n = gpio->idx;

	stm32_gpio_set_value(gpio, val);
	ioclrset32(iobase + GPIO_MODER, GPIO_MODE_MASK(n), GPIO_MODE_OUT(n), 0);
}

static void stm32_gpio_alt_func(struct gpio *gpio, int func)
{
	struct gpiochip *chip = gpio->chip;
	void __iomem* iobase = chip->iobase;
	int n = gpio->idx;

	ioclrset32(iobase + GPIO_AFR(n), GPIO_AF(n, 0xF), GPIO_AF(n, func), 0);
	ioclrset32(iobase + GPIO_MODER, GPIO_MODE_MASK(n), GPIO_MODE_AF(n), 0);
}


static const struct gpio_ops stm32_gpio_ops = {
	.input =	stm32_gpio_input,
	.output =	stm32_gpio_output,
	.set_value =	stm32_gpio_set_value,
	.get_value =	stm32_gpio_get_value,
	.alt_func =	stm32_gpio_alt_func,
	.set_irq =	NULL,	// see EXTI controller
	.pull_up =	NULL,	// TODO
	.pull_on =	NULL,	// TODO
};


static struct stm32_gpio {
	struct gpiochip	gpiochip;
	struct gpio	gpios[16];
	char		name[3];
} stm32_gpios[11];

static void stm32_gpio_init_one(int port)
{
	void __iomem *rcc_base = (void __iomem*) RCC_BASE;
	struct stm32_gpio *gpio = &stm32_gpios[port];
	struct gpiochip *gpiochip;
	void __iomem *iobase;
	char *name = gpio->name;

	// enable the clock for this GPIO: FIXME: do this on gpio_get()
	ioset32(rcc_base + RCC_GPIOENR, RCC_GPIOEN(port));

	name[0] = 'P';
	name[1] = 'A' + port;
	name[2] = '\0';

	iobase = ioremap(GPIO_BASE(port), 0x100);

	// pure GPIO part
	gpiochip = &gpio->gpiochip;
	gpiochip->name = name;
	gpiochip->nbr = 16;
	gpiochip->iobase = iobase;
	gpiochip->ops = &stm32_gpio_ops;
	gpiochip->gpios = gpio->gpios;
	gpiochip_register(gpiochip);
}

static void stm32_gpio_init(void)
{
	for (uint i = 0; i < ARRAY_SIZE(stm32_gpios); i++)
		stm32_gpio_init_one(i);
}
board_core_initcall(stm32_gpio_init);
