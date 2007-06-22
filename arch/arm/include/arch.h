#ifndef _ARCH_ARCH_H_
#define _ARCH_ARCH_H_

//
// CPSR & SPSR
//

#define PSR_MODE_MASK		0x1F
#define PSR_MODE_FIQ		0x11
#define PSR_MODE_IRQ		0x12
#define PSR_MODE_SVC		0x13
#define PSR_MODE_MON		0x16
#define PSR_MODE_ABT		0x17
#define PSR_MODE_HYP		0x1A
#define PSR_MODE_UND		0x1B
#define PSR_MODE_SYS		0x1F

#define PSR_T			0x20	// Thumb enabled when =1
#define PSR_F			0x40	// FIQ disabled when =1
#define PSR_I			0x80	// IRQ disabled when =1
#define PSR_A			0x0100	// Async abort disabled when =1

#define PSR_J			0x01000000
#define PSR_Q			0x08000000
#define PSR_V			0x10000000
#define PSR_C			0x20000000
#define PSR_Z			0x40000000
#define PSR_N			0x80000000


#define PSR_f			0xff000000	// Flags
#define PSR_s			0x00ff0000	// Status
#define PSR_x			0x0000ff00	// Extension
#define PSR_c			0x000000ff	// Control

#endif
