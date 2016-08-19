#ifndef	_ARCH_GCR_H_
#define	_ARCH_GCR_H_

//#define GRC_BASE	0x1fbf8000	// unneeded: see c0_cmgcrbase


#define GCR_CONFIG	0x0000
#define GCR_BASE	0x0008
#define GCR_CTLR	0x0010
#define GCR_CTLR2	0x0018
#define GCR_ACCESS	0x0020
#define GCR_REV		0x0030
#define GCR_ERROR_MASK	0x0040
#define GCR_ERROR_CAUSE	0x0048
#define GCR_ERROR_ADDR	0x0050
#define GCR_ERROR_MULT	0x0058

#define GCR_CUSTOM_BASE	0x0060
#define GCR_CUSTOM_STAT	0x0068
#define GCR_L2SYNC_BASE	0x0070

#define GCR_GIC_BASE	0x0080
#define GCR_GIC_BASE_EN		(1 << 0)

#define GCR_REG0_BASE	0x0090
#define GCR_REG0_MASK	0x0098
#define GCR_REG1_BASE	0x00A0
#define GCR_REG1_MASK	0x00A8
#define GCR_REG2_BASE	0x00B0
#define GCR_REG2_MASK	0x00B8
#define GCR_REG3_BASE	0x00C0
#define GCR_REG3_MASK	0x00C8


#define GCR_CPC_BASE	0x0088
#define GCR_CPC_BASE_EN		(1 << 0)


#define GCR_GIC_STATUS	0x00D0
#define GCR_GIC_STATUS_EX	(1 << 0)

#define GCR_CPC_STATUS	0x00F0
#define GCR_CPC_STATUS_EX	(1 << 0)

#endif
