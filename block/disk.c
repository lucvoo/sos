#include <disk.h>
#include <string.h>
#include <errno.h>
#include <lock.h>
#include "parts.h"


static struct disks {
	struct dlist_head	head;
	struct lock		lock;
} disks = {
	.head = DLIST_HEAD_INIT(disks.head),
	.lock = LOCK_INIT,
};

static struct disk *disk_lookup(const char *name)
{
	struct disk *disk;

	dlist_foreach_entry(disk, &disks.head, list) {
                if (streq(name, disk->name))
                        return disk;
	}

	return NULL;
}

int disk_register(struct disk *disk)
{
	int rc = 0;

	disk->parts_nbr = 0;
	disk->parts = NULL;

	lock_acq(&disks.lock);

	if (disk_lookup(disk->name)) {
		lock_rel(&disks.lock);
		return -EEXIST;
	}

	dlist_add_head(&disks.head, &disk->list);
	lock_rel(&disks.lock);

	rc = disk_partitions(disk);

	return rc;
}
