#ifndef	_NETDEV_H_
#define	_NETDEV_H_

#include <iomem.h>
#include <net/macaddr.h>

struct irqdesc;

struct netdev {
	const char* name;

	struct macaddr macaddr;

	void __iomem*	iobase;
	struct irqdesc*	irq;

	int (*open)(struct netdev *ndev);
	int (*down)(struct netdev *ndev);

	struct netdev *next;
};

int netdev_register(struct netdev *ndev);

#endif
