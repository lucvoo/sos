#ifndef	_FS_H_
#define	_FS_H_

#include <fs-types.h>

int fs_mount(struct blkdev *bdev, enum fs_type, uint flags, const void *options);

#endif
