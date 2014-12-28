#include <iomap.h>
#include <io.h>
#include <arch/memory.h>
#include <arch/hw/mmu.h>
#include <arch/cp15.h>
#include <stringify.h>
#include <errno.h>
#include <stddef.h>


#if 0
static void dump_pgd(const unsigned long *p)
{
	int i;

	for (i = 0; i < PGD_NBR_ENT; i++) {
		unsigned long val = p[i];

		if (!val)
			continue;
		printf("PGD[%03x] = %08x\n", i, p[i]);
	}
}
#endif

static unsigned long *get_pgd(void)
{
	unsigned long pgd;

	asm volatile ("mrc " STRINGIFY(TTBR1(%0)): "=r" (pgd) :: "memory");

	return phys_to_virt(pgd & 0xFFFFF000);
}

static int iomap(unsigned long phys, unsigned long size, unsigned long virt)
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

	return 0;
}

int iomap_init(const struct iomap_desc *d, unsigned int nbr)
{
	int rc = 0;

	for (; nbr; d++, nbr--) {
		rc = iomap(d->phys, d->size, d->virt);
		if (rc) {
			//printf("ioremap() fails for %08x-%08x\n", d->phys, d->phys + d->size - 1);
			break;
		}
	}

	return rc;
}


////////////////////////////////////////////////////////////////////////////////

void __iomem *ioremap(unsigned long phys, unsigned long size)
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
