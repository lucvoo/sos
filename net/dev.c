#include <net/dev.h>
#include <printf.h>


static int ndev_idx;

static struct net_devices {
	struct netdev*	next;
} net_devices;


int netdev_register(struct netdev *ndev)
{
	ndev->next = net_devices.next;
	net_devices.next = ndev;

	snprintf(ndev->ifname, sizeof(ndev->ifname), "eth%d", ndev_idx++);
	ndev->open(ndev);

	return 0;
}
