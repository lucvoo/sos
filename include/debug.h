#ifndef	_DEBUG_H_
#define	_DEBUG_H_

#ifdef	DEBUG
#define	dbg(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
static inline void dbg(const char *fmt, ...)
{
	(void) fmt;
}
#endif

#endif
