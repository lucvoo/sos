#ifndef	_DELAY_H_
#define	_DELAY_H_

void udelay(unsigned int usec);

// FIXME
#define	usleep(D)	udelay(D)

#endif
