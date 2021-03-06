#ifndef	_HW_AM335X_H_
#define	_HW_AM335X_H_

#define	L4_PER_PHYS		0x48000000
#define	L4_PER_SIZE		0x01000000
#define	L4_PER_VIRT		0xF0000000
#define	L4_PER_VADDR(X)		((X) - L4_PER_PHYS + L4_PER_VIRT)

#define	L4_FAST_PHYS		0x4A000000
#define	L4_FAST_SIZE		0x01000000
#define	L4_FAST_VIRT		0xF1000000

#define	L4_WKUP_PHYS		0x44C00000
#define	L4_WKUP_SIZE		0x00400000
#define	L4_WKUP_VIRT		0xF2000000


#define INTC_BASE		0x48200000
#define	IOBASE_WDT1		0x44E35000
#define	IOBASE_DMTIMER1_1MS	0x44E31000
#define	IOBASE_DMTIMER0		0x44E05000
#define	IOBASE_DMTIMER2		0x48040000
#define	IOBASE_DMTIMER3		0x48042000
#define	IOBASE_DMTIMER4		0x48044000
#define	IOBASE_DMTIMER5		0x48046000
#define	IOBASE_DMTIMER6		0x48048000
#define	IOBASE_DMTIMER7		0x4804A000

#endif
