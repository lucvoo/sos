#ifndef	_DISK_H_
#define	_DISK_H_

#include <types/sector_t.h>
#include <dlist.h>


struct disk;

struct disk_ops {
	int	(*read)(struct disk *, void *buff, sector_t, uint nbr);
	int	(*write)(struct disk *, sector_t, uint nbr, const void *buff);
	int	(*erase)(struct disk *, sector_t, uint nbr);
};


struct disk {
	const char		*name;
	sector_t		size;
	struct disk_ops		*ops;
	void			*priv;

	struct dlist		list;
};

int disk_register(struct disk *disk);


#endif
