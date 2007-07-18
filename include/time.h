#ifndef	_TIME_H_
#define	_TIME_H_

#include <types/timespec.h>

static inline int timespec_equal(struct timespec *a, struct timespec *b)
{
	return (a->tv_sec == b->tv_sec) && (a->tv_nsec == b->tv_nsec);
}

static inline int timespec_compare(struct timespec *a, struct timespec *b)
{
	if (a->tv_sec < b->tv_sec)
		return -1;
	if (a->tv_sec > b->tv_sec)
		return 1;
	return a->tv_nsec - b->tv_nsec;
}

#endif
