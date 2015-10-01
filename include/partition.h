#ifndef	_PARTITION_H_
#define	_PARTITION_H_

#include <types/sector_t.h>


struct partition {
	sector_t		off;
	sector_t		nbr;
	u8			idx;
};

#endif
