#define MIDR(R)		p15, 0, R,  c0,  c0, 0
#define CTR(R)		p15, 0, R,  c0,  c0, 1
#define TCMTR(R)	p15, 0, R,  c0,  c0, 2
#define TLBTR(R)	p15, 0, R,  c0,  c0, 3
#define MPIDR(R)	p15, 0, R,  c0,  c0, 5
#define REVIDR(R)	p15, 0, R,  c0,  c0, 6
#define CCSIDR(R)	p15, 1, R,  c0,  c0, 0
#define CLIDR(R)	p15, 1, R,  c0,  c0, 1
#define AIDR(R)		p15, 1, R,  c0,  c0, 7
#define CSSELR(R)	p15, 2, R,  c0,  c0, 0
#define SCTLR(R)	p15, 0, R,  c1,  c0, 0
#define SCR(R)		p15, 0, R,  c1,  c1, 0
#define NSACR(R)	p15, 0, R,  c1,  c1, 2
#define TTBR0(R)	p15, 0, R,  c2,  c0, 0
#define TTBR1(R)	p15, 0, R,  c2,  c0, 1
#define TTBCR(R)	p15, 0, R,  c2,  c0, 2
#define DFSR(R)		p15, 0, R,  c5,  c0, 0
#define DACR(R)		p15, 0, R,  c3,  c0, 0
#define IFSR(R)		p15, 0, R,  c5,  c0, 1
#define DFAR(R)		p15, 0, R,  c6,  c0, 0
#define IFAR(R)		p15, 0, R,  c6,  c0, 2

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

//
#define PRRR(R)		p15, 0, R, c10,  c2, 0
#define NMRR(R)		p15, 0, R, c10,  c2, 1

#define VBAR(R)		p15, 0, R, c12,  c0, 0
#define MVBAR(R)	p15, 0, R, c12,  c0, 1

#define ISR(R)		p15, 0, R, c12,  c1, 0

#define CONTEXTIDR(R)	p15, 0, R, c13,  c0, 1
#define TPIDRURW(R)	p15, 0, R, c13,  c0, 2
#define TPIDRUR0(R)	p15, 0, R, c13,  c0, 3
#define TPIDRPRW(R)	p15, 0, R, c13,  c0, 4

// 
#define CNTFRQ(R)	p15, 0, R, c14,  c0, 0
