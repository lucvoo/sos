#ifndef	_IO_H_
#define	_IO_H_

#include <arch/io.h>
#include <types/paddr_t.h>


void __iomem *ioremap(paddr_t phys, unsigned long size);

static inline void iounmap(void __iomem *addr, unsigned long size)
{
	// FIXME
}


#ifdef CONFIG_NOMMU
static inline void __iomem *__ioremap_nop(paddr_t phys, unsigned long size)
{
	return (void __iomem *) phys;
}

#define	ioremap(A, S)	__ioremap_nop(A, S)
#endif


static inline void iowrite32_flush(void __iomem *ioaddr, u32 val)
{
	iowrite32(ioaddr, val);
	ioread32(ioaddr);		// sync with write's early ack
}

static inline void ioclr32(void __iomem *addr, u32 clr)
{
	u32 val = ioread32(addr);

	val &= ~clr;
	iowrite32(addr, val);
}

static inline void ioset32(void __iomem *addr, u32 set)
{
	u32 val = ioread32(addr);

	val |= set;
	iowrite32(addr, val);
}

static inline void ioclrset32(void __iomem *addr, u32 clr, u32 set, uint off)
{
	u32 val = ioread32(addr);

	val &= ~(clr << off);
	val |=  (set << off);
	iowrite32(addr, val);
}

static inline u64 ioread2x32(void __iomem *addrh, void __iomem *addrl)
{
	u32 hi0, hi1, low;

	do {
		hi0 = ioread32(addrh);
		low = ioread32(addrl);
		hi1 = ioread32(addrh);
	} while (hi1 != hi0);

	return (((u64) hi0) << 32) + low;
}

#endif
