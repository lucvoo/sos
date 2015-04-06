#ifndef	_GPIO_H_
#define	_GPIO_H_


struct gpio;
struct irqdesc;

enum gpio_trigger_type {
	GPIO_TRIG_LEVEL_HIGH,
	GPIO_TRIG_LEVEL_LOW,
	GPIO_TRIG_EDGE_RISE,
	GPIO_TRIG_EDGE_FALL,
	GPIO_TRIG_EDGE_BOTH,
};


struct gpio*	gpio_get(const char *name, unsigned int idx);
void		gpio_put(struct gpio *gpio);

void gpio_input(struct gpio *gpio);
void gpio_output(struct gpio *gpio, int value);
void gpio_set_value(struct gpio *gpio, int value);
int  gpio_get_value(struct gpio *gpio);

void gpio_interrupt(struct gpio *gpio, enum gpio_trigger_type type);

void gpio_alt_func(struct gpio *gpio, unsigned int func);

void gpio_pull_up(struct gpio *gpio, int up);
void gpio_pull_on(struct gpio *gpio, int on);

struct irqdesc *gpio_get_irq(struct gpio *gpio);

#endif
