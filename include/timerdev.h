#ifndef _TIMER_DEV_H_
#define _TIMER_DEV_H_

#include <iomem.h>
#include <hz.h>


struct timerdev {
	const char *name;
	int (*next_abs)(struct timerdev *td, unsigned long val);
	int (*next_rel)(struct timerdev *td, unsigned long val);
	unsigned long (*now)(struct timerdev *td);
	unsigned long freq;

	void __iomem *base;

	void (*handler)(struct timerdev *td);
};

int timerdev_register(struct timerdev *td);
unsigned long timerdev_read(void);


// standard DSR method to use for timers
struct irqdesc;
int timerdev_dsr(struct irqdesc *desc, uint count, void *data);

#endif
