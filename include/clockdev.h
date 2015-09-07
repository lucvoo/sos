#ifndef _CLOCKDEV_H_
#define _CLOCKDEV_H_


struct clockdev {
	u64		(*read)(struct clockdev *cd);
	u32		rate;
	u8		bits;

	const char	*name;
	struct clockdev	*next;
};


int clockdev_register(struct clockdev *cd);

#endif
