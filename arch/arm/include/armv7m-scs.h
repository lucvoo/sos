#ifndef _ARCH_ARMV7M_SCS_H_
#define _ARCH_ARMV7M_SCS_H_

#define SCS_BASE	0xE000E000
#define SCS_SIZE	    0x1000

#define SCB_BASE	(SCS_BASE + 0x0D00)
#define CPUID		(SCB_BASE +   0x00)
#define ICSR		(SCB_BASE +   0x04)
#define VTOR		(SCB_BASE +   0x08)
#define AIRCR		(SCB_BASE +   0x0C)
#define SCR		(SCB_BASE +   0x10)
#define CCR		(SCB_BASE +   0x14)
#define SHPR1		(SCB_BASE +   0x18)
#define SHPR2		(SCB_BASE +   0x1C)
#define SHPR3		(SCB_BASE +   0x20)
#define SHCSR		(SCB_BASE +   0x24)
#define USGFAULTENA		(1 << 18)
#define BUSFAULTENA		(1 << 17)
#define MEMFAULTENA		(1 << 16)

#define CFSR		(SCB_BASE +   0x28)
#define HFSR		(SCB_BASE +   0x2C)
#define DFSR		(SCB_BASE +   0x30)
#define MMFAR		(SCB_BASE +   0x34)
#define BFAR		(SCB_BASE +   0x38)
#define AFSR		(SCB_BASE +   0x3C)
#define CPACR		(SCB_BASE +   0x88)

#endif
