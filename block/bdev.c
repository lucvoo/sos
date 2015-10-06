#include <blkdev.h>
#include <printf.h>
#include <string.h>
#include <errno.h>
#include <lock.h>
#include <disk.h>


static struct block_devs {
	struct dlist_head	head;
	struct lock		lock;
} block_devs = {
	.head = DLIST_HEAD_INIT(block_devs.head),
	.lock = LOCK_INIT,
};

static struct blkdev *bdev_lookup(const char *name, uint idx)
{
	struct blkdev *bdev;

	dlist_foreach_entry(bdev, &block_devs.head, list) {
		pr_info("< %s.%d\n", bdev->disk->name, bdev->part->idx);
                if (streq(name, bdev->disk->name) && (idx == bdev->part->idx))
                        return bdev;
	}

	return NULL;
}

int bdev_register(struct blkdev *bdev)
{
	if (!bdev->disk)
		return -EINVAL;
	if (!bdev->part)
		return -EINVAL;

	lock_acq(&block_devs.lock);
	if (bdev_lookup(bdev->disk->name, bdev->part->idx)) {
		lock_rel(&block_devs.lock);
		return -EEXIST;
	}
	dlist_add_head(&block_devs.head, &bdev->list);
	lock_rel(&block_devs.lock);

	return 0;
}

struct blkdev *bdev_get(const char *diskname, uint part_idx)
{
	struct blkdev *bdev;

	lock_acq(&block_devs.lock);
	bdev = bdev_lookup(diskname, part_idx);

	// FIXME: need ref counting

	lock_rel(&block_devs.lock);

	return bdev;
}
