#ifndef	_NET_ETHER_H_
#define	_NET_ETHER_H_

struct skb;


void ether_dump_frame(const void *frame, unsigned int len);

unsigned int eth_type(struct skb *skb);

#endif
