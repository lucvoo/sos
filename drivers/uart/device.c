#include "driver.h"
#include <lock.h>
#include <uartdev.h>
#include <errno.h>
#include <string.h>


static struct uart_devices {
	struct lock	lock;
	struct uart*	next;
} uart_devices;

static struct uart *lookup_dev(const char *name)
{
	struct uart *dev;

	for (dev = uart_devices.next; dev; dev = dev->next)
		if (streq(name, dev->name))
			break;

	return dev;
}

int uart_register_dev(struct uart *p)
{
	const char *name = p->name;
	int rc = -EEXIST;

	lock_acq(&uart_devices.lock);
	if (!lookup_dev(name)) {
		p->next = uart_devices.next;
		uart_devices.next = p;
		rc = 0;
	}
	lock_rel(&uart_devices.lock);

	return rc;
}

struct uart* uart_get_dev(const char *name)
{
	struct uart *p;

	// Not locked but OK
	p = lookup_dev(name);

	return p;
}
