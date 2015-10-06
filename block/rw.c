#include <blkdev.h>
#include <disk.h>
#include <partition.h>


int bread(struct blkdev *bdev, void *buff, sector_t sec, uint nbr)
{
	const struct partition *part = bdev->part;
	struct disk *disk = bdev->disk;

	return disk->ops->read(disk, buff, sec + part->off, nbr);
}

int bwrite(struct blkdev *bdev, sector_t sec, uint nbr, const void *buff)
{
	const struct partition *part = bdev->part;
	struct disk *disk = bdev->disk;

	return disk->ops->write(disk, sec + part->off, nbr, buff);
}
