#include <clockdev.h>
#include <lock.h>


// FIXME: per-cpu (if possible)
static struct {
	struct lock	lock;
	struct clockdev	*devs;		// all clocks
	struct clockdev *sys;		// for system time, scheduling, timer, ...
} clockdevs;


int clockdev_register(struct clockdev *cd)
{
	// FIXME: use some selection of the "best" clock

	lock_acq_irq(&clockdevs.lock);
	cd->next = clockdevs.devs;
	clockdevs.devs = cd;
	clockdevs.sys = cd;
	lock_rel_irq(&clockdevs.lock);

	return 0;
}
