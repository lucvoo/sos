#ifndef _ARCH_ARCH_H_
#define _ARCH_ARCH_H_

/*
 * SCTLR bits (CP#15 CR1)
 */
#define	SCTLR_M		(1 << 0)	// MMU enable
#define	SCTLR_A		(1 << 1)	// Alignment abort enable
#define	SCTLR_C		(1 << 2)	// Dcache enable
#define	SCTLR_W		(1 << 3)	// Write buffer enable
#define	SCTLR_P		(1 << 4)	// 32-bit exception handler
#define	SCTLR_D		(1 << 5)	// 32-bit data address range
#define	SCTLR_L		(1 << 6)	// Implementation defined
#define	SCTLR_B		(1 << 7)	// Big endian
#define	SCTLR_S		(1 << 8)	// System MMU protection
#define	SCTLR_R		(1 << 9)	// ROM MMU protection
#define	SCTLR_F		(1 << 10)	// Implementation defined
#define	SCTLR_Z		(1 << 11)	// Implementation defined
#define	SCTLR_I		(1 << 12)	// Icache enable
#define	SCTLR_V		(1 << 13)	// Vectors relocated to 0xffff0000
#define	SCTLR_RR	(1 << 14)	// Round Robin cache replacement
#define	SCTLR_L4	(1 << 15)	// LDR pc can set T bit
#define	SCTLR_DT	(1 << 16)
#define	SCTLR_IT	(1 << 18)
#define	SCTLR_ST	(1 << 19)
#define	SCTLR_FI	(1 << 21)	// Fast interrupt (lower latency mode)
#define	SCTLR_U		(1 << 22)	// Unaligned access operation
#define	SCTLR_XP	(1 << 23)	// Extended page tables
#define	SCTLR_VE	(1 << 24)	// Vectored interrupts

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
