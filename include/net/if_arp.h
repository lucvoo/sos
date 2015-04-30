#ifndef	_NET_IF_ARP_H_
#define	_NET_IF_ARP_H_

#include <types/kint.h>

// Hardware type
#define ARPHRD_ETHER	1

// Operation code
#define	ARPOP_REQUEST	1
#define	ARPOP_REPLY	2


struct arphdr {
	__be16		hrd;		// hardware address type
	__be16		pro;		// protocol address type
	u8		hln;		// hardware address length
	u8		pln;		// protocol address length
	__be16		op;		// opcode
/*
	u8		sha[hln];	// sender hardware address
	u8		spa[pln];	// sender protocol address
	u8		tha[hln];	// target hardware address
	u8		tpa[pln];	// target protocom address
*/
	u8		addresses[0];
};


#include <net/dev.h>
static inline unsigned int arp_packet_len(struct netdev *dev)
{
	switch (ARPHRD_ETHER) {		// Only one supported for now
	default:
						// 2 HW & 2 IP addresses
		return sizeof(struct arphdr) + 2 * (netdev_addr_len(dev) + 4);
	}
}

#endif
