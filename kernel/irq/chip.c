#include <irqdesc.h>
#include <irqchip.h>
#include <lock.h>
#include <string.h>


static struct irqchip *chips;
static struct lock chips_lock;

static struct irqchip *lookup_chip(const char *name)
{
	struct irqchip *chip;
	unsigned long flags;

	flags = lock_acq_save(&chips_lock);
	for (chip = chips; chip; chip = chip->next) {
		// This is kludgy!
		// => If no name is given we return the first chip registered
		//    which should be the 'root' controller.
		//    This should only be used by __do_IRQ()
		if (!name && !chip->next)
			break;
		if (!name)
			continue;

		if (strcmp(chip->name, name) == 0)
			break;
	}
	lock_rel_rest(&chips_lock, flags);

	return chip;
}

static void insert_chip(struct irqchip *chip)
{
	unsigned long flags;

	flags = lock_acq_save(&chips_lock);
	chip->next = chips;
	chips = chip;
	lock_rel_rest(&chips_lock, flags);
}


struct irqdesc *irq_get_desc(const char *name, unsigned int irq)
{
	struct irqchip *chip;
	struct irqdesc *desc;

	chip = lookup_chip(name);
	if (!chip)
		return NULL;

	if (irq >= chip->irq_nbr)
		return NULL;

	desc = &chip->descs[irq];

	return desc;
}


void irqchip_init(struct irqdesc *parent, struct irqchip *chip)
{
	unsigned int i;

	insert_chip(chip);
	chip->parent = parent;

	for (i = 0; i < chip->irq_nbr; i++) {
		struct irqdesc *desc = &chip->descs[i];

		desc->chip = chip;
		desc->irq = i;
		desc->handler = chip->default_handler;
		lock_init(&desc->lock);
	}
}
