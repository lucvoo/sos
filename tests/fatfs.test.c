#include <kapi/kapi.h>
#include <thread.h>
#include <sched.h>
#include <blkdev.h>
#include <fs.h>



static struct thread t __uninit;

static void fatfs_test(void* data)
{
	struct blkdev *bdev = data;
	int rc;

	rc = fs_mount(bdev, FS_TYPE_FAT,0, NULL);
	pr_note("rc = %d\n", rc);

	//printf("TEST fatfs: fail = %u, succeed = %i\n", ko, ok);
	thread_sleep();
}

void kapi_start(void)
{
	struct blkdev *bdev;

	bdev = bdev_get("mmc0", 1);
	pr_note("bdev = %p\n", bdev);
	thread_create(&t, 2, fatfs_test, bdev, NULL, 0);
	thread_start(&t);
}
