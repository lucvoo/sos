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


//
// SCTLR bits (CP#15 CR1)
//
#define	SCTLR_M		(1 << 0)	// MMU enable
#define	SCTLR_A		(1 << 1)	// Alignment check enable
#define	SCTLR_C		(1 << 2)	// D/U cache enable
#define	SCTLR_W		(1 << 3)	// Write buffer enable		(< ARMv7, RAO/SBOP for ARMv7)
#define	SCTLR_BE	(1 << 5)	// CP15 dmb, dsb & isb enabled	(ARMv7, optional)
#define	SCTLR_B		(1 << 7)	// Big endian			(< ARMv7, ARMv7 RAZ/SBOP)
#define	SCTLR_S		(1 << 8)	// System MMU protection	(< ARMv7, RAZ/SBOP for ARMv7)
#define	SCTLR_R		(1 << 9)	// ROM MMU protection		(< ARMv7, RAZ/SBOP for ARMv7)
#define	SCTLR_SW	(1 << 10)	// SWP & SWPB enable
#define	SCTLR_Z		(1 << 11)	// branch prediction enable
#define	SCTLR_I		(1 << 12)	// Icache enable
#define	SCTLR_V		(1 << 13)	// high Vectors
#define	SCTLR_RR	(1 << 14)	// Round Robin cache replacement
#define	SCTLR_DT	(1 << 16)	//				(< ARMv7, RAO/SBOP)
#define	SCTLR_HA	(1 << 17)	// Hardware Access flag		(ARMv7)
#define	SCTLR_WXN	(1 << 19)	//				(if virtualization)
#define	SCTLR_UWXN	(1 << 20)	//				(if virtualization)
#define	SCTLR_FI	(1 << 21)	// Fast Interrupt (lower latency mode)
#define	SCTLR_U		(1 << 22)	// Unaligned access operation	(< ARMv7, RAO/SBOP)
#define	SCTLR_XP	(1 << 23)	// eXtended Page tables		(< ARMv7)
#define	SCTLR_VE	(1 << 24)	// VEctored interrupts
#define	SCTLR_EE	(1 << 25)	// Exception Endianness
#define	SCTLR_NMFI	(1 << 27)	// Non-Maskable FIQ
#define	SCTLR_TRE	(1 << 28)	// Tex Remap Enable
#define	SCTLR_AFE	(1 << 29)	// Access Flag Enable
#define	SCTLR_TE	(1 << 30)	// Thumb Exception enable

#endif
