#include "gpiochip.h"
#include <irqdesc.h>

void gpio_input(struct gpio *gpio)
{
	gpio->chip->ops->input(gpio);
}

void gpio_output(struct gpio *gpio, int value)
{
	gpio->chip->ops->output(gpio, value);
}

void gpio_set_value(struct gpio *gpio, int value)
{
	gpio->chip->ops->set_value(gpio, value);
}

int  gpio_get_value(struct gpio *gpio)
{
	return gpio->chip->ops->get_value(gpio);
}

void gpio_interrupt(struct gpio *gpio, enum gpio_trigger_type type)
{
	gpio->chip->ops->set_irq(gpio, type);
}

void gpio_alt_func(struct gpio *gpio, unsigned int func)
{
	gpio->chip->ops->alt_func(gpio, func);
}

void gpio_pull_up(struct gpio *gpio, int up)
{
	gpio->chip->ops->pull_up(gpio, up);
}

void gpio_pull_on(struct gpio *gpio, int on)
{
	gpio->chip->ops->pull_on(gpio, on);
}


struct irqdesc* gpio_get_irq(struct gpio *gpio)
{
	return &gpio->chip->irqs[gpio->idx];
}
