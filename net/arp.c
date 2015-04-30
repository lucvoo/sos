#include <net/if_arp.h>
#include <net/proto.h>
#include <net/skb.h>
#include <net/dev.h>
#include <byteorder.h>
#include <string.h>


void net_arp_rx(struct skb *skb)
{
	const struct arphdr *arp;
	unsigned int hw_type;
	const unsigned char *sha;
	const unsigned char *tha;
	__be32 sip, tip;


	if (skb->casting == PACKET_CASTING_OTHER)
		goto drop;

	if (skb->len < arp_packet_len(skb->dev))
		goto drop;

	arp = skb->data;

#if 0
	pr_info("hrd = %04x\n\tpro = %04x\n\thln = %u\n\tpln = %u\n\top  = %u\n"
		, be16_to_cpu(arp->hrd), be16_to_cpu(arp->pro)
		, arp->hln, arp->pln
		, be16_to_cpu(arp->op)
		);
#endif

	if (arp->hln != netdev_addr_len(skb->dev))
		goto drop;
	if (arp->pln != 4)
		goto drop;

	hw_type = ARPHRD_ETHER;		// FIXME: only HW device supported for now
	switch (hw_type) {
	case ARPHRD_ETHER:
		if (arp->hrd != cpu_to_be16(ARPHRD_ETHER))
			goto drop;
		if (arp->pro != cpu_to_be16(ETH_P_IPV4))
			goto drop;
		break;
	}

	switch(be16_to_cpu(arp->op)) {
	case ARPOP_REQUEST:
	case ARPOP_REPLY:
		break;
	default:
		goto drop;
	}

	sha = &arp->addresses[0];
	memcpy(&sip, &sha[arp->hln], 4);
	tha = sha + arp->hln + 4;
	memcpy(&tip, &tha[arp->hln], 4);

	pr_info("sha = %pM, sip = %pI4, tha = %pM, tip = %pI4\n", sha, &sip, tha, &tip);

drop:
	skb_free(skb);
	return;
}
