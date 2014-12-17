#include <iomap.h>
#include <arch/memory.h>
#include <arch/hw/mmu.h>
#include <arch/cp15.h>
#include <stringify.h>
#include <errno.h>


static unsigned long *get_pgd(void)
{
	unsigned long pgd;

	asm volatile ("mrc " STRINGIFY(TTBR1(%0)): "=r" (pgd) :: "memory");

	return (void *) (pgd & 0xFFFFF000);
}

static int iomap(const struct iomap_desc *d)
{
	unsigned long *pgd = get_pgd();
	unsigned long *p;
	unsigned long f;
	unsigned int i;

	// only accept sections
	if (d->phys & PGD_SECT_MASK)
		return -EINVAL;
	if (d->size & PGD_SECT_MASK)
		return -EINVAL;
	if (d->virt & PGD_SECT_MASK)
		return -EINVAL;

	// check if the entry are empty
	p = &pgd[d->virt >> PGD_SECT_SHIFT];
	i = d->size >> PGD_SECT_SHIFT;
	for (; i--;) {
		if ((p[i] & PGD_TYPE_MASK) != PGD_TYPE_FAULT)
			return -EINVAL;
	}

	// do the actual mapping
	i = d->size >> PGD_SECT_SHIFT;
	f = d->phys | PGD_INIT_IO;
	for (; i--; f += PGD_SECT_SIZE) {
		*p++ = f;
	}

	return 0;
}

int iomap_init(const struct iomap_desc *d, unsigned int nbr)
{
	int rc = 0;

	for (; nbr; d++, nbr--) {
		rc = iomap(d);
		if (rc)
			break;
	}

	return rc;
}
