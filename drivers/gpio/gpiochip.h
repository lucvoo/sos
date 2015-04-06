#ifndef	_GPIOCHIP_H_
#define	_GPIOCHIP_H_

#include <gpio.h>
#include <iomem.h>

struct gpiochip;


struct gpio {
	struct gpiochip	*chip;
	unsigned int	idx;
};

struct gpio_ops {
	void		(*input)	(struct gpio *gpio);
	void		(*output)	(struct gpio *gpio, int value);
	void		(*set_value)	(struct gpio *gpio, int value);
	int		(*get_value)	(struct gpio *gpio);

	void		(*set_irq)	(struct gpio *gpio, enum gpio_trigger_type type);

	void		(*alt_func)	(struct gpio *gpio, int function);
	void		(*pull_up)	(struct gpio *gpio, int up_down);
	void		(*pull_on)	(struct gpio *gpio, int on_off);
};

struct gpiochip {
	const char*		name;
	unsigned int		nbr;
	unsigned long		features;

	void __iomem*		iobase;
	const struct gpio_ops*	ops;
	struct gpio*		gpios;
	struct irqdesc*		irqs;

	struct gpiochip*	next;
};


int gpiochip_register(struct gpiochip *chip);

#endif
