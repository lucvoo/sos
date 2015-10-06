#ifndef	_FILESYSTEM_H_
#define	_FILESYSTEM_H_


struct blkdev;

struct filesystem {
	const char	*name;
	u8		type;

	int (*mount)(struct blkdev *bdev, uint flags, const void *options);


	struct filesystem	*next;
};


int filesystem_register(struct filesystem *);

#endif
