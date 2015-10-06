#include <filesystem.h>
#include <fs-types.h>
#include <fs.h>
#include <errno.h>
#include <lock.h>


static struct {
	struct filesystem	*next;
	struct lock		lock;
} filesystems = {
	.lock = LOCK_INIT,
};


static struct filesystem *fs_lookup(enum fs_type type)
{
	struct filesystem *fs;

	for (fs = filesystems.next; fs; fs = fs->next) {
		if (fs->type == type)
			break;
	}

	return fs;
}

int filesystem_register(struct filesystem *fs)
{
	int rc = -EINVAL;

	lock_acq(&filesystems.lock);

	if (!fs_lookup(fs->type)) {
		fs->next = filesystems.next;
		filesystems.next = fs;
		rc = 0;
	}

	lock_rel(&filesystems.lock);

	return rc;
}

static struct filesystem *filesystem_get(enum fs_type type)
{
	struct filesystem *fs;

	lock_acq(&filesystems.lock);
	fs = fs_lookup(type);
	lock_rel(&filesystems.lock);

	return fs;
}


int fs_mount(struct blkdev *bdev, enum fs_type type, uint flags, const void *options)
{
	struct filesystem *fs;
	int rc;

	fs = filesystem_get(type);
	if (!fs)
		return -EINVAL;

	rc = fs->mount(bdev, flags, options);
	return rc;
}
