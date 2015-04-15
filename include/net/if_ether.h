#ifndef	_NET_IF_ETHER_H_
#define	_NET_IF_ETHER_H_


#define ETH_ALEN	6		// Address length
#define ETH_HLEN	14		// Header length
#define ETH_ZLEN	60		// Minimum frame length
#define ETH_PLEN	1500		// Maximum payload length
#define ETH_FLEN	1514		// Maximum frame length (4 more for VLAN)


// Ethernet types/protocols
#define ETH_P_IPV4	0x0800		//
#define ETH_P_ARP	0x0806		//
#define ETH_P_RARP	0x8035		// Reverse ARP
#define ETH_P_8021Q	0x8100          // 802.1Q VLAN Extended Header
#define ETH_P_IPV6	0x86dd		//
#define ETH_P_PPP_DISC	0x8863		// PPPoE Discovery
#define ETH_P_PPP_SESS	0x8864		// PPPoE Session
#define ETH_P_PTP	0x88f7		// Precision Time Protocol

#define ETH_P_802_3_MIN	0x0600		// len/type limit
#define ETH_P_802_3	0x0001		// Dummy type for 802.3 frames
#define ETH_P_802_2	0x0004		// 802.2 frames


#include <net/macaddr.h>
#include <types/kint.h>

struct ethhdr {
	struct macaddr	dst;
	struct macaddr	src;
	__be16		proto;		// type/protocol
};

#endif
