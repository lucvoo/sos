#ifndef _TIMER_DEV_H_
#define _TIMER_DEV_H_

#include <iomem.h>


struct timerdev {
	const char *name;
	int (*program)(struct timerdev *td, unsigned long val);
	unsigned long (*now)(struct timerdev *td);
	unsigned long freq;

	void __iomem *base;

	void (*handler)(struct timerdev *td);
};

int timerdev_register(struct timerdev *td);
unsigned long timerdev_read(void);

#endif
