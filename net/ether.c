#include <net/if_ether.h>
#include <net/ether.h>
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
