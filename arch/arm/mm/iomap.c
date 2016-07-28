#include <iomap.h>
#include <io.h>
#include <arch/cacheflush.h>
#include <arch/memory.h>
#include <arch/hw/mmu.h>
#include <arch/cp15.h>
#include <stringify.h>
#include <errno.h>
#include <stddef.h>
#include <barrier.h>


#if 0
static void dump_pgd(const unsigned long *p)
{
	uint pf = -1;
	uint pa = 0;
	int new = 0;
	int i;

	for (i = 0; i < PGD_NBR_ENT; i++) {
		unsigned long val = p[i];
		ulong f = val & 0x00002FFF;
		ulong a = val >> PGD_SECT_SHIFT;

		if ((f != pf) || ((a == 0) != (pa == 0)) || (a != 0 && (a != (pa + 1)))) {
			printf("PGD[%03x] = %08x\n", i, p[i]);
			pf = f;
			new = 1;
		} else if (new) {
			printf("\t   ...\n", i, p[i]);
			new = 0;
		}

		pa = a;
	}
}

static void test_mmu(ulong va)
{
	ulong pa;
	asm volatile ("mcr " STRINGIFY(ATS1CPR(%0)) :: "r" (va));
	asm volatile ("mrc " STRINGIFY(PAR(%0)) :  "=r" (pa));
	printf("VA %08lx -> PA %08lx\n", va, pa);
}
#endif

static unsigned long *get_pgd(void)
{
	unsigned long pgd;

	asm volatile ("mrc " STRINGIFY(TTBR1(%0)): "=r" (pgd) :: "memory");

	return phys_to_virt(pgd & 0xFFFFC000);
}

static int iomap(paddr_t phys, unsigned long size, unsigned long virt)
{
	unsigned long *pgd = get_pgd();
	unsigned int i;

	// only accept sections
	if (phys & PGD_SECT_MASK)
		return -EINVAL;
	phys >>= PGD_SECT_SHIFT;

	if (size & PGD_SECT_MASK)
		return -EINVAL;
	size >>= PGD_SECT_SHIFT;

	if (virt & PGD_SECT_MASK)
		return -EINVAL;
	virt >>= PGD_SECT_SHIFT;

#if 0
	// check if the entry are empty
	for (i = size; i--;) {
		if ((pgd[virt + i] & PGD_TYPE_MASK) != PGD_TYPE_FAULT)
			return -EINVAL;
	}
#endif

	// do the actual mapping
	for (i = size; i--; ) {
		pgd[virt + i] = (phys + i) << PGD_SECT_SHIFT | PGD_INIT_IO;
	}

	dcache_clean_range(&pgd[virt], size * 4);
	return 0;
}

int iomap_init(const struct iomap_desc *d, unsigned int nbr)
{
	int err = 0;

	for (err = 0; nbr; d++, nbr--) {
		if (iomap(d->phys, d->size, d->virt)) {
			pr_dbg("error for %08x:%08x\n", d->phys, d->size);
			err++;
		}
	}

	asm ("mcr " STRINGIFY(TLBIALL(r0)));
	isb();

	return err ? -EINVAL : 0;
}


////////////////////////////////////////////////////////////////////////////////

void __iomem *ioremap(paddr_t phys, unsigned long size)
{
	unsigned long off = phys & PGD_SECT_MASK;
	unsigned long *pgd = get_pgd();
	unsigned long res = 0;
	unsigned int i;


	// lookup matching entry
	phys = phys >> PGD_SECT_SHIFT;
	i = PGD_NBR_ENT;
	for (; i--;) {
		if ((pgd[i] >> PGD_SECT_SHIFT) == phys) {
			res = (i << PGD_SECT_SHIFT) | off;
			break;
		}
	}

	// FIXME: find an entry & iomap it
	return (void __iomem *)res;
}
