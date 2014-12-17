#ifndef	_IOMAP_H_
#define	_IOMAP_H_

struct iomap_desc {
	unsigned long	phys;
	unsigned long	size;
	unsigned long	virt;
};


int iomap_init(const struct iomap_desc *, unsigned int nbr);

#endif
