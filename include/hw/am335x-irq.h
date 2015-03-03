#ifndef _HW_AM335X_IRQ_H_
#define _HW_AM335X_IRQ_H_

#include <io.h>
#include <hw/am335x.h>


#define INTC_REVISION		0x000
#define INTC_SYSCONFIG		0x010
#define INTC_SYSSTATUS		0x014
#define INTC_SIR_IRQ		0x040
#define INTC_SIR_FIQ		0x044
#define INTC_CONTROL		0x048
#define INTC_PROTECTION		0x04C
#define INTC_IDLE		0x050
#define INTC_IRQ_PRIORITY	0x060
#define INTC_FIQ_PRIORITY	0x064
#define INTC_THRESHOLD		0x068

#define INTC_ITR(X)		(0x080 + 0x020*(X))
#define INTC_MIR(X)		(0x084 + 0x020*(X))
#define INTC_MIR_CLEAR(X)	(0x088 + 0x020*(X))
#define INTC_MIR_SET(X)		(0x08C + 0x020*(X))
#define INTC_ISR_SET(X)		(0x090 + 0x020*(X))
#define INTC_ISR_CLEAR(X)	(0x094 + 0x020*(X))
#define INTC_PENDING_IRQ(X)	(0x098 + 0x020*(X))
#define INTC_PENDING_FIQ(X)	(0x09C + 0x020*(X))

#define	ACTIVE_IRQ_MASK		((1 << 7) -1)
#define	SPURIOUS_IRQ_MASK	(~ACTIVE_IRQ_MASK)

#endif
