#ifndef	_IOMAP_H_
#define	_IOMAP_H_

#include <types/paddr_t.h>

struct iomap_desc {
	paddr_t		phys;
	unsigned long	size;
	unsigned long	virt;
};


int iomap_init(const struct iomap_desc *, unsigned int nbr);

#endif
