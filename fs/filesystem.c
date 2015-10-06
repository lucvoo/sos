#include <filesystem.h>
#include <lock.h>


static struct {
	struct filesystem	*next;
	struct lock		lock;
} filesystems = {
	.lock = LOCK_INIT,
};


int filesystem_register(struct filesystem *fs)
{
	lock_acq(&filesystems.lock);

	// FIXME: should lookup & check if no name duplicate

	fs->next = filesystems.next;
	filesystems.next = fs;

	lock_rel(&filesystems.lock);

	return 0;
}
