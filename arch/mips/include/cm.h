#ifndef	_ARCH_CM_H_
#define	_ARCH_CM_H_

#include <arch/gcr.h>


#define GIC_BASE	0x1bdc0000
#define GIC_SIZE	   0x20000

#define CPC_BASE	0x1bde0000
#define CPC_SIZE	    0x6000


#ifndef __ASSEMBLY__
/**
 * Give the current cpu access to @core CM's registers
 */
void mips_cm_get(uint core);
void mips_cm_put(void);
#endif

#endif
