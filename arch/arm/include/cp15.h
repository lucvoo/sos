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
#define PAR(R)		p15, 0, R,  c7,  c4, 0

// cache operations

// TLB operation
#define	TLBIALLIS(R)	p15, 0, R,  c8,  c3, 0
#define	TLBIMVAIS(R)	p15, 0, R,  c8,  c3, 1
#define	TLBIASIDIS(R)	p15, 0, R,  c8,  c3, 2
#define	TLBIMVAAIS(R)	p15, 0, R,  c8,  c3, 3
#define	TLBIALL(R)	p15, 0, R,  c8,  c7, 0
#define	TLBIMVA(R)	p15, 0, R,  c8,  c7, 1
#define	TLBIASID(R)	p15, 0, R,  c8,  c7, 2
#define	TLBIMVAA(R)	p15, 0, R,  c8,  c7, 3

// performance monitors

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
