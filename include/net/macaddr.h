#ifndef	_NET_MACADDR_H_
#define	_NET_MACADDR_H_

#include <types/kint.h>

struct macaddr {
	union {
		u8	byte[6];
		u16	half[6/2];
	};
};


static inline int macaddr_is_multicast(const struct macaddr *addr)
{
	return addr->byte[0] & 0x01;
}

static inline int macaddr_is_local(const struct macaddr *addr)
{
	return addr->byte[0] & 0x02;
}

static inline int macaddr_is_zero(const struct macaddr *addr)
{
	return (addr->half[0] | addr->half[1] | addr->half[2]) == 0x0000;

}

static inline int macaddr_is_broadcast(const struct macaddr *addr)
{
	return (addr->half[0] & addr->half[1] & addr->half[2]) == 0xffff;

}

static inline int macaddr_is_valid(const struct macaddr *addr)
{
	return !macaddr_is_zero(addr) && !macaddr_is_multicast(addr);

}


static inline int macaddr_is_equal(const struct macaddr *a1, const struct macaddr *a2)
{
	const u16 *h1 = &a1->half[0];
	const u16 *h2 = &a2->half[0];

	return ((h1[0] ^ h2[0]) | (h1[1] ^ h2[1]) | (h1[2] ^ h2[2])) == 0;
}

#endif
