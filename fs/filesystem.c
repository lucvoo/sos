#include <filesystem.h>
#include <fs-types.h>
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
