#ifndef	__ASSEMBLY__
#include <stringify.h>
#endif

#define	cp_read(R)	({ ulong r; asm volatile("mrc " STRINGIFY(R(%0)) : "=r" (r)); r; })
#define	cp_write(R,V)	({ asm volatile("mcr " STRINGIFY(R(%0)) :: "r" (V)); })
#define	cp_read64(R)	({ u64 r; asm volatile("mrrc " STRINGIFY(R(%Q0, %R0)) : "=r" (r)); r; })
#define	cp_write64(R,V)	({ asm volatile("mcrr " STRINGIFY(R(%Q0, %R0)) :: "r" (V)); })


#define	MIDR(R)		p15, 0, R,  c0,  c0, 0	// Main ID Register
#define	CTR(R)		p15, 0, R,  c0,  c0, 1	// Cache Type Register
#define	TCMTR(R)	p15, 0, R,  c0,  c0, 2	// TCM Type Register IMPLEMENTATION DEFINED
#define	TLBTR(R)	p15, 0, R,  c0,  c0, 3	// TLB Type Register IMPLEMENTATION DEFINED
#define	MPIDR(R)	p15, 0, R,  c0,  c0, 5	// Multiprocessor Affinity Register
#define	MPIDR_CORE(R)		(((R) >> 0) & 0xff)
#define	MPIDR_CLUSTER(R)	(((R) >> 8) & 0xff)
#define	REVIDR(R)	p15, 0, R,  c0,  c0, 6	// Revision ID Register a
#define	ID_PFR0(R)	p15, 0, R,  c0,  c1, 0	// Processor Feature Register 0
#define	ID_PFR1(R)	p15, 0, R,  c0,  c1, 1	// Processor Feature Register 1
#define	ID_PFR1_GENERIC_TIMER	(1 << 16)
#define	ID_PFR1_VITUALIZATION	(1 << 12)
#define	ID_PFR1_TRUSTZONE	(1 <<  4)
#define	ID_PFR1_NSACR_RFR	(2 <<  4)

#define	ID_DFR0(R)	p15, 0, R,  c0,  c1, 2	// Debug Feature Register 0
#define	ID_AFR0(R)	p15, 0, R,  c0,  c1, 3	// Auxiliary Feature Register 0
#define	ID_MMFR0(R)	p15, 0, R,  c0,  c1, 4	// Memory Model Feature Register 0
#define	ID_MMFR1(R)	p15, 0, R,  c0,  c1, 5	// Memory Model Feature Register 1
#define	ID_MMFR2(R)	p15, 0, R,  c0,  c1, 6	// Memory Model Feature Register 2
#define	ID_MMFR3(R)	p15, 0, R,  c0,  c1, 7	// Memory Model Feature Register 3
#define	ID_ISAR0(R)	p15, 0, R,  c0,  c2, 0	// ISA Feature Register 0
#define	ID_ISAR1(R)	p15, 0, R,  c0,  c2, 1	// ISA Feature Register 1
#define	ID_ISAR2(R)	p15, 0, R,  c0,  c2, 2	// ISA Feature Register 2
#define	ID_ISAR3(R)	p15, 0, R,  c0,  c2, 3	// ISA Feature Register 3
#define	ID_ISAR4(R)	p15, 0, R,  c0,  c2, 4	// ISA Feature Register 4
#define	ID_ISAR5(R)	p15, 0, R,  c0,  c2, 5	// ISA Feature Register 5
#define	CCSIDR(R)	p15, 1, R,  c0,  c0, 0	// Cache Size ID Registers
#define	CLIDR(R)	p15, 1, R,  c0,  c0, 1	// Cache Level ID Register
#define	AIDR(R)		p15, 1, R,  c0,  c0, 7	// Auxiliary ID Register IMPLEMENTATION DEFINED
#define	CSSELR(R)	p15, 2, R,  c0,  c0, 0	// Cache Size Selection Register

// system control registers
#define	SCTLR(R)	p15, 0, R,  c1,  c0, 0	// System Control Register
#define	ACTLR(R)	p15, 0, R,  c1,  c0, 1	// Auxiliary Control Register IMPLEMENTATION DEFINED
#define  ACTLR_SMP	(1 << 6)
#define	CPACR(R)	p15, 0, R,  c1,  c0, 2	// Coprocessor Access Control Register
#define	SCR(R)		p15, 0, R,  c1,  c1, 0	// Secure Configuration Register †
#define	SDER(R)		p15, 0, R,  c1,  c1, 1	// Secure Debug Enable Register †
#define	NSACR(R)	p15, 0, R,  c1,  c1, 2	// Non-Secure Access Control Register †
						// †: implemented only as part of security extension

// memory protection & control
#define	TTBR0(R)	p15, 0, R,  c2,  c0, 0	// Translation Table Base Register 0
#define	TTBR1(R)	p15, 0, R,  c2,  c0, 1	// Translation Table Base Register 1
#define	TTBCR(R)	p15, 0, R,  c2,  c0, 2	// Translation Table Base Control Register
#define	DACR(R)		p15, 0, R,  c3,  c0, 0	// Domain Access Control Register

// memory system fault registers
#define	DFSR(R)		p15, 0, R,  c5,  c0, 0	// Data Fault Status Register
#define	IFSR(R)		p15, 0, R,  c5,  c0, 1	// Intruction Fault Status Register
#define	ADFSR(R)	p15, 0, R,  c5,  c1, 0	// Auxiliary DFSR
#define	AIFSR(R)	p15, 0, R,  c5,  c1, 1	// Auxiliary IFSR
#define	DFAR(R)		p15, 0, R,  c6,  c0, 0	// Data Fault Address Register
#define	IFAR(R)		p15, 0, R,  c6,  c0, 2	// Instruction Fault Address Register

// cache maintenance & address translation
#define	ICIALLUIS(R)	p15, 0, R,  c7,  c1, 0	// Invalidate all instruction caches to PoU Inner Shareable ø
#define	BPIALLIS(R)	p15, 0, R,  c7,  c1, 6	// Invalidate all branch predictors Inner Shareable ø
#define	PAR(R)		p15, 0, R,  c7,  c4, 0	// Physical Address Register
#define	ICIALLU(R)	p15, 0, R,  c7,  c5, 0	// Invalidate all instruction caches to PoU
#define	ICIMVAU(R)	p15, 0, R,  c7,  c5, 1	// Invalidate instruction caches by MVA to PoU
#define	CP15ISB(R)	p15, 0, R,  c7,  c5, 4	// Instruction Synchronization Barrier operation
#define	BPIALL(R)	p15, 0, R,  c7,  c5, 6	// Invalidate all branch predictors
#define	BPIMVA(R)	p15, 0, R,  c7,  c5, 7	// Invalidate MVA from branch predictors
#define	DCIMVAC(R)	p15, 0, R,  c7,  c6, 1	// Invalidate D/U cache line by MVA to PoC
#define	DCISW(R)	p15, 0, R,  c7,  c6, 2	// Invalidate D/U cache line by set/way
#define	ATS1CPR(R)	p15, 0, R,  c7,  c8, 0	// PL1 read translation
#define	ATS1CPW(R)	p15, 0, R,  c7,  c8, 1	// PL1 write translation
#define	ATS1CUR(R)	p15, 0, R,  c7,  c8, 2	// unprivileged read translation
#define	ATS1CUW(R)	p15, 0, R,  c7,  c8, 3	// unprivileged write translation
#define	ATS12NSOPR(R)	p15, 0, R,  c7,  c8, 4	// PL1 read translation †
#define	ATS12NSOPW(R)	p15, 0, R,  c7,  c8, 5	// PL1 write translation †
#define	ATS12NSOUR(R)	p15, 0, R,  c7,  c8, 6	// unprivileged read translation †
#define	ATS12NSOUW(R)	p15, 0, R,  c7,  c8, 7	// unprivileged write translation †
#define	DCCMVAC(R)	p15, 0, R,  c7, c10, 1	// Clean D/U cache line by MVA to PoC
#define	DCCSW(R)	p15, 0, R,  c7, c10, 2	// Clean D/U cache line by set/way
#define	CP15DSB(R)	p15, 0, R,  c7, c10, 4	// Data Synchronization Barrier operation
#define	CP15DMB(R)	p15, 0, R,  c7, c10, 5	// Data Memory Barrier operation
#define	DCCMVAU(R)	p15, 0, R,  c7, c11, 1	// Clean D/U cache line by MVA to PoU
#define	DCCIALL(R)	p15, 0, R,  c7, c14, 0	// Clean and invalidate D/U entire cache (ARMv6)
#define	DCCIMVAC(R)	p15, 0, R,  c7, c14, 1	// Clean and invalidate D/U cache line by MVA to PoC
#define	DCCISW(R)	p15, 0, R,  c7, c14, 2	// Clean and invalidate D/U cache line by set/way
						// ø: introduced as part of multiprocessor extension
						// †: implemented only as part of security extension

// TLB operation
#define	TLBIALLIS(R)	p15, 0, R,  c8,  c3, 0	// Invalidate entire TLB IS ø
#define	TLBIMVAIS(R)	p15, 0, R,  c8,  c3, 1	// Invalidate unified TLB entry by MVA and ASID IS ø
#define	TLBIASIDIS(R)	p15, 0, R,  c8,  c3, 2	// Invalidate unified TLB by ASID match IS ø
#define	TLBIMVAAIS(R)	p15, 0, R,  c8,  c3, 3	// Invalidate unified TLB entry by MVA all ASID IS ø
#define	ITLBIALL(R)	p15, 0, R,  c8,  c5, 0	// invalidate instruction TLB
#define	ITLBIMVA(R)	p15, 0, R,  c8,  c5, 1	// invalidate instruction TLB entry by MVA and ASID
#define	ITLBIASID(R)	p15, 0, R,  c8,  c5, 2	// invalidate instruction TLB by ASID match
#define	DTLBIALL(R)	p15, 0, R,  c8,  c6, 0	// invalidate data TLB
#define	DTLBIMVA(R)	p15, 0, R,  c8,  c6, 1	// invalidate data TLB entry by MVA and ASID
#define	DTLBIASID(R)	p15, 0, R,  c8,  c6, 2	// invalidate data TLB by ASID match
#define	TLBIALL(R)	p15, 0, R,  c8,  c7, 0	// invalidate unified TLB
#define	TLBIMVA(R)	p15, 0, R,  c8,  c7, 1	// invalidate unified TLB entry by MVA and ASID
#define	TLBIASID(R)	p15, 0, R,  c8,  c7, 2	// invalidate unified TLB by ASID match
#define	TLBIMVAA(R)	p15, 0, R,  c8,  c7, 3	// invalidate unified TLB entries by MVA all ASID ø
						// ø: introduced as part of multiprocessor extension

// Multi-processor & L2 memory system (only Cortex A7, A15 & A17)
#define	L2CTLR(R)	p15, 1, R,  c9,	c0, 2	// L2 control (MP info)
#define	L2ECTLR(R)	p15, 1, R,  c9,	c0, 3	// L2 extended control

// performance monitors
#define	PMCR(R)		p15, 0, R,  c9,	c12, 0
#define	PMCNTENSET(R)	p15, 0, R,  c9,	c12, 1
#define	PMCNTENCLR(R)	p15, 0, R,  c9,	c12, 2
#define	PMOVSR(R)	p15, 0, R,  c9,	c12, 3
#define	PMSWINC(R)	p15, 0, R,  c9,	c12, 4
#define	PMSELR(R)	p15, 0, R,  c9,	c12, 5
#define	PMCEID0(R)	p15, 0, R,  c9,	c12, 6
#define	PMCEID1(R)	p15, 0, R,  c9,	c12, 7
#define	PMCCNTR(R)	p15, 0, R,  c9,	c13, 0
#define	PMXEVTYPER(R)	p15, 0, R,  c9,	c13, 1
#define	PMXEVCNTR(R)	p15, 0, R,  c9,	c13, 2
#define	PMUSERENR(R)	p15, 0, R,  c9,	c14, 0
#define	PMINTENSET(R)	p15, 0, R,  c9,	c14, 1
#define	PMINTENCLR(R)	p15, 0, R,  c9,	c14, 2
#define	PMOVSSET(R)	p15, 0, R,  c9,	c14, 3

// memory remapping
#define	PRRR(R)		p15, 0, R, c10,  c2, 0	// Primary Region Remap Register
#define	NMRR(R)		p15, 0, R, c10,  c2, 1	// Normal Memory Remap Register

// 
#define	VBAR(R)		p15, 0, R, c12,  c0, 0	// Vector Base Address Register †
#define	MVBAR(R)	p15, 0, R, c12,  c0, 1	// Monitor Vector Base Address Register †
#define	ISR(R)		p15, 0, R, c12,  c1, 0	// Interupt Status Register
						// †: implemented only as part of security extension

// process, context & thread ID
#define	CONTEXTIDR(R)	p15, 0, R, c13,  c0, 1	// Context ID Register
#define	TPIDRURW(R)	p15, 0, R, c13,  c0, 2	// Thread/Process ID User RW
#define	TPIDRURO(R)	p15, 0, R, c13,  c0, 3	// Thread/Process ID User RO
#define	TPIDRPRW(R)	p15, 0, R, c13,  c0, 4	// Thread/Process ID Privileged RW

// Generic Timer Extension
#define	CNTFRQ(R)	p15, 0, R, c14,  c0, 0	// Counter Frequency Register
#define	CNTKCTL(R)	p15, 0, R, c14,  c1, 0	// Timer PL1 Control Register
#define	CNTP_TVAL(R)	p15, 0, R, c14,  c2, 0	// PL1 Physical Timer Value register
#define	CNTP_CTL(R)	p15, 0, R, c14,  c2, 1	// PL1 Physical Control register
#define	CNTV_TVAL(R)	p15, 0, R, c14,  c3, 0	// PL1 Virtual Timer Value register
#define	CNTV_CTL(R)	p15, 0, R, c14,  c3, 1	// PL1 Virtual Control register

#define	CNTPCNT(Q,R)	p15, 0, Q, R, c14	// PL1 Physical Counter register
#define	CNTP_CVAL(Q,R)	p15, 2, Q, R, c14	// Physical Timer Compare Value register
#define	CNTVCNT(Q,R)	p15, 1, Q, R, c14	// PL1 Virtual Counter register
#define	CNTV_CVAL(Q,R)	p15, 3, Q, R, c14	// Virtual Timer Compare Value register
