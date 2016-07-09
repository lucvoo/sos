#ifndef	_TIMER_H_
#define	_TIMER_H_

#include <hz.h>
#include <dlist.h>


struct timer {
	struct dlist	node;
	unsigned long	exp;	// For now use a 32 bits: even at 32KHz it will "only" wrap around every 36 hours

	void (*action)(void *data);
	void *data;
};


void timer_add_abs(struct timer *t);
void timer_add_rel(struct timer *t, ulong ticks);
void timer_del(struct timer *t);

#endif
