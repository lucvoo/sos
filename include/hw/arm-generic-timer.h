#ifndef	_HW_ARM_GENERIC_TIMER_H_
#define	_HW_ARM_GENERIC_TIMER_H_


#define	CNTKCTL_PL0PTEN		(1 << 9)
#define	CNTKCTL_PL0VTEN		(1 << 8)
#define	CNTKCTL_EVNTEN		(1 << 2)
#define	CNTKCTL_PL0VCTEN	(1 << 1)
#define	CNTKCTL_PL0PCTEN	(1 << 0)
#define	CNTKCTL_PRESERVE	0xfffffc00

#define	CNTX_CTL_ISTATUS	(1 << 2)
#define	CNTX_CTL_IMASK		(1 << 1)
#define	CNTX_CTL_ENABLE		(1 << 0)

#endif
