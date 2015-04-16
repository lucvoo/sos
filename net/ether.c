#include <net/if_ether.h>
#include <net/ether.h>
#include <net/skb.h>
#include <byteorder.h>


void ether_dump_frame(const void *frame, unsigned int len)
{
	const struct ethhdr *hdr = frame;

	printf("ethernet frame:\n"
		"\tlen: 0x%04x (%d)\n"
		"\tdst: %pM\n"
		"\tsrc: %pM\n"
		"\ttyp: %04x\n", len, len, hdr->dst, hdr->src, be16_to_cpu(hdr->proto));
}


unsigned int eth_type(struct skb *skb)
{
	const struct ethhdr *hdr;
	unsigned int proto;

	hdr = (const void *) skb->data;

	// TODO: packet type

	proto = be16_to_cpu(hdr->proto);
	if (likely(proto >= ETH_P_802_3_MIN))
		return proto;

	// could this be an IPX packet on raw 802.3?
	// (0xFFFF is an illegal DSAP/SSAP)
	if (*((const u16*) &hdr[1]) == 0xFFFF)
		return ETH_P_802_3;

	return ETH_P_802_2;
}
