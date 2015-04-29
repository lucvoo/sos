#ifndef	_NET_PROTO_H_
#define	_NET_PROTO_H_

#include <net/if_ether.h>

struct skb;

void net_arp_rx(struct skb *skb);

#endif
