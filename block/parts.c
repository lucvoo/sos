#include <partition.h>
#include <kmalloc.h>
#include <blkdev.h>
#include <errno.h>
#include <disk.h>
#include "parts.h"


int disk_partitions(struct disk *disk)
{
	int rc;
	int i;

	rc = part_mbr(disk);
	if (rc < 0)
		return rc;

	// create a block device for each partitions
	for (i = 0; i <= disk->parts_nbr; i++) {
		struct blkdev *bdev;

		bdev = knew(struct blkdev, GFP_KERN);
		if (!bdev) {
			// FIXME: leak previous bdevs
			return -ENOMEM;
		}

		bdev->disk = disk;
		bdev->part = &disk->parts[i];
		rc = bdev_register(bdev);
		if (rc) {
			// FIXME: leak previous bdevs
			return rc;
		}
	}

	return rc;
}
