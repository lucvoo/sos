#ifndef	_ARCH_CPC_H_
#define	_ARCH_CPC_H_

#include <arch/cm.h>

#define	CPC_GL_OFFSET	0x0000		// Global
#define	CPC_CL_OFFSET	0x2000		// Core Local
#define	CPC_CO_OFFSET	0x4000		// Core Other
#define	CPC_CL(R)	(CPC_CL_OFFSET + (R))
#define	CPC_CO(R)	(CPC_CO_OFFSET + (R))

#ifndef __ASSEMBLY__
ulong mips_cpc_read(uint reg);
void mips_cpc_write(uint reg, ulong val);
void mips_cpc_set(uint reg, ulong bits);
void mips_cpc_clr(uint reg, ulong bits);

#define	mips_cpc_cl_read(R)	mips_cpc_read(CPC_CL(R))
#define	mips_cpc_cl_write(R, V)	mips_cpc_write(CPC_CL(R), V)

#define	mips_cpc_co_read(R)	mips_cpc_read(CPC_CO(R))
#define	mips_cpc_co_write(R, V)	mips_cpc_write(CPC_CO(R), V)

/**
 * Give the current cpu access to @core CPC's registers
 */
void mips_cpc_get(uint core);
void mips_cpc_put(void);
#endif


#define	CPC_CX_CMD		0x000
#define		CPC_CX_CMD_CLOCKOFF	1
#define		CPC_CX_CMD_POWERDOWN	2
#define		CPC_CX_CMD_POWERUP	3
#define		CPC_CX_CMD_RESET	4

#define	CPC_CX_STAT_CFG		0x008
#define		CPC_CX_SEQSTATE(X)	(((X) >> 19) & 0xf)
#define		CPC_CX_SEQSTATE_D0	0x0
#define		CPC_CX_SEQSTATE_U0	0x1
#define		CPC_CX_SEQSTATE_U1	0x2
#define		CPC_CX_SEQSTATE_U2	0x3
#define		CPC_CX_SEQSTATE_U3	0x4
#define		CPC_CX_SEQSTATE_U4	0x5
#define		CPC_CX_SEQSTATE_U5	0x6
#define		CPC_CX_SEQSTATE_U6	0x7
#define		CPC_CX_SEQSTATE_D1	0x8
#define		CPC_CX_SEQSTATE_D3	0x9
#define		CPC_CX_SEQSTATE_D2	0xa

#define	CPC_CX_OTHER		0x010
#define		CPC_CX_OTHER_CORE(X)	((X) << 16)

#endif
