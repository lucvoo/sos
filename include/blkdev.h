#ifndef	_BLKDEV_H_
#define	_BLKDEV_H_

#include <types/sector_t.h>
#include <dlist.h>


struct disk;
struct partition;

struct blkdev {
	struct disk		*disk;
	const struct partition	*part;

	struct dlist		list;
};


int bdev_register(struct blkdev *bdev);
struct blkdev *bdev_get(const char *diskname, uint part_idx);

#endif
