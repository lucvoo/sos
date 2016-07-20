#ifndef _ARCH_TIMER_H_
#define _ARCH_TIMER_H_

#include <bits.h>


#define	CNTHCTL_EVNTI		BITS(4, 4)
#define	CNTHCTL_EVNTDIR		(1 << 3)
#define	CNTHCTL_EVNTEN		(1 << 2)
#define	CNTHCTL_EL1PCEN		(1 << 1)
#define	CNTHCTL_EL1PCTEN	(1 << 0)

#endif
