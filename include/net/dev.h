#ifndef	_NETDEV_H_
#define	_NETDEV_H_

#include <iomem.h>
#include <net/macaddr.h>

#define	IFNAMESIZ	10

struct skb;
struct irqdesc;

struct netdev_stats {
	unsigned long	rx_errors_fifo;
	unsigned long	rx_errors_crc;
	unsigned long	rx_errors_length;

	unsigned long	rx_packets;
	unsigned long	rx_bytes;

	unsigned long	tx_packets;
	unsigned long	tx_bytes;
};

struct netdev {
	struct macaddr macaddr;
	char		ifname[IFNAMESIZ];
	unsigned long	link:1;

	void __iomem*	iobase;
	struct irqdesc*	irq;

	unsigned int	tx_state;

	int (*open)(struct netdev *ndev);
	int (*poll)(struct netdev *ndev);
	int (*down)(struct netdev *ndev);
#define	NETDEV_TX_OK	0
#define	NETDEV_TX_BUSY	1
	int (*send)(struct netdev *ndev, struct skb *skb);

#ifdef	CONFIG_NET_STATS
	struct netdev_stats	stats;
#endif

	struct netdev *next;
};

int netdev_register(struct netdev *ndev);
struct netdev *netdev_get(const char *ifname);
int netdev_poll(struct netdev *ndev);


void netif_rx(struct skb *skb);


#ifdef	CONFIG_NET_STATS
#define	netdev_stats_add(dev, item, val)	(dev)->stats.item += val
#else
#define	netdev_stats_add(dev, item, val)	do ; while (0)
#endif
#define	netdev_stats_inc(dev, item)		netdev_stats_add(dev, item, 1)

#endif
