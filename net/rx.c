#include <net/proto.h>
#include <net/dev.h>
#include <net/skb.h>
#include <softirq.h>
#include <irq_stat.h>
#include <init.h>


static void netif_rx_process(struct skb *skb)
{

	switch (skb->proto) {
	case ETH_P_ARP:
		return net_arp_rx(skb);
	default:
		// TODO
		break;
	}

	pr_info("skb: len = %4u, proto = %04x\n", skb->len, skb->proto);

	skb_free(skb);
}


// FIXME: should this be per-cpu, per-device?
static struct skb_queue rx_queue = {
	.head = DLIST_HEAD_INIT(rx_queue.head),
	.len  = 0,
	.lock = LOCK_UNLOCKED,
};


void netif_rx(struct skb *skb)
{
	// This function must always be called in interrupt context,
	// so no need to disable interrupts here.
	// FIXME: enforce interrupt context

	lock_acq(&rx_queue.lock);

	skb_enqueue_tail(&rx_queue, skb);

	lock_rel(&rx_queue.lock);

	softirq_raise(SOFTIRQ_NETRX);
}


static void softirq_netrx(struct softirq_action* action)
{
	struct skb *skb;

	lock_acq_irq(&rx_queue.lock);
	while ((skb = skb_dequeue_head(&rx_queue))) {

		lock_rel_irq(&rx_queue.lock);
		netif_rx_process(skb);
		lock_acq_irq(&rx_queue.lock);
	}
	lock_rel_irq(&rx_queue.lock);
}

static void init_softirq_netrx(void)
{
	softirq_register(SOFTIRQ_NETRX, softirq_netrx, NULL);
}
pure_initcall(init_softirq_netrx);
