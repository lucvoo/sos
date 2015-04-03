#include <kapi/kapi.h>
#include <net/dev.h>
#include <sched.h>


#ifndef	HZ
#define	HZ		(32*1024)
#endif


static void netloop(void* data)
{
	const char *ifname = data;
	struct netdev *ndev;

	ndev = netdev_get(ifname);
	if (!ndev) {
		printf("ERR: no '%s' found\n", ifname);
		return;
	}

	while (1) {
		netdev_poll(ndev);

		thread_schedule_timeout(HZ);
	}
}


void kapi_start(void)
{
	static struct thread thr __uninit;
	static char ifname[] = "eth0";

	thread_create(&thr, 2, netloop, ifname, 0, 0);
	thread_start(&thr);
}
