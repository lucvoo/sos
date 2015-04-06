#include "gpiochip.h"
#include <lock.h>
#include <errno.h>
#include <string.h>


static struct gpio_chips {
	struct lock		lock;
	struct gpiochip*	list;
} gpio_chips;


static struct gpiochip *gpio_lookup(const char *name)
{
	struct gpiochip *chip;

	for (chip = gpio_chips.list; chip; chip = chip->next)
		if (streq(name, chip->name))
			break;

	return chip;
}

int gpiochip_register(struct gpiochip *chip)
{
	const char *name = chip->name;
	int rc = -EEXIST;

	lock_acq(&gpio_chips.lock);
	if (!gpio_lookup(name)) {
		chip->next = gpio_chips.list;
		gpio_chips.list = chip;
		rc = 0;
	}
	lock_rel(&gpio_chips.lock);

	return rc;
}


struct gpio* gpio_get(const char *name, unsigned int idx)
{
	struct gpiochip *chip;
	struct gpio *gpio;

	chip = gpio_lookup(name);
	if (!chip)
		return NULL;
	if (idx >= chip->nbr)
		return NULL;

	gpio = &chip->gpios[idx];
	gpio->chip = chip;
	gpio->idx = idx;
	return gpio;
}
