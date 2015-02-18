#ifndef	_MIPS_REGS_H_
#define	_MIPS_REGS_H_


#define	c0_context	$4, 0

#define	c0_hwrena	$7, 0

#define	c0_badvaddr	$8, 0

#define	c0_count	$9, 0

#define	c0_compare	$11, 0

#define	c0_status	$12, 0
#define		ST0_IE		(1 << 0)
#define		ST0_EXL		(1 << 1)
#define		ST0_ERL		(1 << 2)
#define		ST0_KSU		(3 << 3)
#define		ST0_UM		(1 << 4)
#define		ST0_IM0		(1 << 8)
#define		ST0_IM1		(1 << 9)
#define		ST0_IM2		(1 <<10)
#define		ST0_IM3		(1 <<11)
#define		ST0_IM4		(1 <<12)
#define		ST0_IM5		(1 <<13)
#define		ST0_IM6		(1 <<14)
#define		ST0_IM7		(1 <<15)
#define		ST0_NMI		(1 <<19)
#define		ST0_SR		(1 <<20)
#define		ST0_TS		(1 <<21)
#define		ST0_BEV		(1 <<22)
#define		ST0_FR		(1 <<26)
#define		ST0_RP		(1 <<27)
#define		ST0_CU0		(1 <<28)
#define		ST0_CU1		(1 <<29)

#define	c0_intctl	$12, 1
#define		INTCTL_VS_OFF	5
#define		INTCTL_VS_MSK	((1 <<5) - 1)

#define	c0_srsctl	$12, 2

#define	c0_srsmap	$12, 3

#define	c0_cause	$13, 0

#define	c0_epc		$14, 0

#define	c0_prid		$15, 0

#define	c0_ebase	$15, 1

#define	c0_config	$16, 0
#define		CFG0_K0		(1 << 0)
#define		CFG0_VI		(1 << 3)
#define		CFG0_MT		(1 << 7)
#define		CFG0_AR		(1 <<10)
#define		CFG0_AT		(1 <<13)
#define		CFG0_BE		(1 <<15)
#define		CFG0_M		(1 <<31)

#define	c0_config1	$16, 1
#define		CFG1_FP		(1 << 0)
#define		CFG1_EP		(1 << 1)
#define		CFG1_CA		(1 << 2)
#define		CFG1_WR		(1 << 3)
#define		CFG1_PC		(1 << 4)
#define		CFG1_MMU_SIZE1	(1 <<25)
#define		CFG1_M		(1 <<31)

#define	c0_config2	$16, 2
#define		CFG2_M		(1 <<31)

#define	c0_config3	$16, 3
#define		CFG3_VINT	(1 << 5)
#define		CFG3_VEIC	(1 << 6)
#define		CFG3_M		(1 <<31)

#define	c0_config6	$16, 6

#define	c0_config7	$16, 7

#define	c0_errorepc	$30, 0

#endif
