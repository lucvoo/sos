#include <net/dev.h>
#include <net/skb.h>


void netif_rx(struct skb *skb)
{
	// TODO

	skb_free(skb);
}
