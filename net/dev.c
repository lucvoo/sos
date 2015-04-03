#include <net/dev.h>
#include <string.h>
#include <printf.h>


static int ndev_idx;

static struct net_devices {
	struct netdev*	next;
} net_devices;


static struct netdev *lookup(const char *ifname)
{
	struct netdev *dev;

	for (dev = net_devices.next; dev; dev = dev->next) {
		if (streq(ifname, dev->ifname))
			break;
	}

	return dev;
}

int netdev_register(struct netdev *ndev)
{
	ndev->next = net_devices.next;
	net_devices.next = ndev;

	snprintf(ndev->ifname, sizeof(ndev->ifname), "eth%d", ndev_idx++);
	ndev->open(ndev);

	return 0;
}

struct netdev *netdev_get(const char *ifname)
{
	return lookup(ifname);
}

int netdev_poll(struct netdev *ndev)
{
	int rc = 0;

	if (ndev->poll)
		rc = ndev->poll(ndev);

	return rc;
}
