#include <arch/mmu.h>
#include <arch/mmu-hw.h>
#include <arch/msr.h>
#include <page.h>


#define	PGD_BLK(ADR, MAI) (	\
	PTE_AF	|		\
	PTE_ISH	|		\
	(ADR & ~(PGD_SIZE-1)) |	\
	PTE_MAIDX(MAI) |	\
	PTD_TYPE_BLK)

// FIXME: this can only be done if the different
//	  memory region are not in the same block
static ulong identity_pgd[PGD_NBR_ENT] __aligned(PGD_NBR_ENT * 8) = {
	[IOMEM_PHYS / PGD_SIZE] = PGD_BLK(IOMEM_PHYS, MT_DEV),	// IO
	[PHYS_ADDR  / PGD_SIZE] = PGD_BLK(PHYS_ADDR,  MT_MEM),	// start of RAM
};

ulong *__arch_mmu_setup(void);	// used by startup.S
ulong *__arch_mmu_setup(void)
{
	u64 prange;

	msr_setval(mair_el1,
		MAIR(MT_DEV,    MAIR_DEV_nGnRE) |
		MAIR(MT_MEM,    MAIR_MEM_WB) |
		MAIR(MT_MEM_NC, MAIR_MEM_NC) |
		0);

	prange = msr_getval(id_aa64mmfr0_el1) & 0b111;

	msr_setval(tcr_el1,
		TCR_IPS(prange) |
		TCR_TG1(TCR_TG) |
		TCR_SH1(TCR_SH_IS) |
		TCR_RGN1(TCR_RGN_WBWA) |
		TCR_T1SZ(64 - VIRT_BITS) |
		TCR_TG0(TCR_TG) |
		TCR_SH0(TCR_SH_IS) |
		TCR_RGN0(TCR_RGN_WBWA) |
		TCR_T0SZ(64 - VIRT_BITS));

	// Warning: the address returned is PC-relative
	//	Whether it's a physical or virtual address depend
	//	on the context this function is called.
	return identity_pgd;
}
