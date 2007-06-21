#ifndef _ARCH_ARCH_H_
#define _ARCH_ARCH_H_

//
// CPSR & SPSR
//

#define PSR_I			0x80	// IRQ disabled when =1
#define PSR_F			0x40	// FIQ disabled when =1
#define PSR_T			0x20	// Thumb enabled when =1

#define PSR_MODE_MASK		0x1F
#define PSR_MODE_FIQ		0x11
#define PSR_MODE_IRQ		0x12
#define PSR_MODE_SVC		0x13
#define PSR_MODE_MON		0x16
#define PSR_MODE_ABT		0x17
#define PSR_MODE_HYP		0x1A
#define PSR_MODE_UND		0x1B
#define PSR_MODE_SYS		0x1F

#endif
