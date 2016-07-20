#ifndef	_ARCH_A53_H_
#define	_ARCH_A53_H_


#define	ACTLR_CPUECTLR_EL1	(1 << 1)


#define	cpuectlr_el1	S3_1_C15_C2_1
#define	CPUECTLR_SMPEN		(1 << 6)


#ifdef	__ASSEMBLY__

/*
 * startup_cpu: very early CPU initialization
 *              possibly done at EL3 and/or EL2
 * @t0, @t1: temporary registers
 */
.macro	startup_cpu, t0, t1
#ifdef	CONFIG_PM_POWERDOWN
	// allow access to cpuectlr_el1 (NS = 0)
	mrs	\t0, actlr_el3
	orr	\t1, \t0, ACTLR_CPUECTLR_EL1
	msr	actlr_el3, \t0

	// allow access to cpuectlr_el1 (NS = 1)
	mrs	\t0, actlr_el2
	orr	\t1, \t0, ACTLR_CPUECTLR_EL1
	msr	actlr_el2, \t0
#endif

	// set cpuectrl_el1.smpen (enable SMP coherency and more)
	mrs	\t0, cpuectlr_el1
	orr	\t1, \t0, CPUECTLR_SMPEN
	msr	cpuectlr_el1, \t1
.endm

#endif	// __ASSEMBLY__

#endif
