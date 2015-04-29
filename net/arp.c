#include <net/if_arp.h>
#include <net/proto.h>
#include <net/skb.h>
#include <byteorder.h>


void net_arp_rx(struct skb *skb)
{
	const struct arphdr *arp;

	arp = skb->data;

	pr_info("hrd = %04x\n\tpro = %04x\n\thln = %u\n\tpln = %u\n\top  = %u\n"
		, be16_to_cpu(arp->hrd), be16_to_cpu(arp->pro)
		, arp->hln, arp->pln
		, be16_to_cpu(arp->op)
		);
}
