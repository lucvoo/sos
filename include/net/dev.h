#ifndef	_NETDEV_H_
#define	_NETDEV_H_

#include <iomem.h>
#include <net/macaddr.h>

#define	IFNAMESIZ	10

struct skb;
struct irqdesc;

struct netdev {
	struct macaddr macaddr;
	char		ifname[IFNAMESIZ];
	unsigned long	link:1;

	void __iomem*	iobase;
	struct irqdesc*	irq;

	int (*open)(struct netdev *ndev);
	int (*poll)(struct netdev *ndev);
	int (*down)(struct netdev *ndev);

	struct netdev *next;
};

int netdev_register(struct netdev *ndev);
struct netdev *netdev_get(const char *ifname);
int netdev_poll(struct netdev *ndev);


void netif_rx(struct skb *skb);

#endif
