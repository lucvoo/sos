#ifndef	_ARCH_EXCEPTIONS_H_
#define	_ARCH_EXCEPTIONS_H_


struct eframe {
	unsigned long	r[32];
	unsigned long	hi;
	unsigned long	lo;

	unsigned long	status;
	unsigned long	cause;
	unsigned long	epc;

	unsigned long	padding;
};

#endif
