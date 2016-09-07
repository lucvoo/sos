#include <arch/iomap.h>
#include <arch/cpc.h>
#include <io.h>


ulong mips_cpc_read(uint reg)
{
	void __iomem *cpcbase = __ioremap(CPC_BASE);

	return ioread32(cpcbase + reg);
}

void mips_cpc_write(uint reg, ulong val)
{
	void __iomem *cpcbase = __ioremap(CPC_BASE);

	iowrite32(cpcbase + reg, val);
}

void mips_cpc_clr(uint reg, ulong bits)
{
	void __iomem *cpcbase = __ioremap(CPC_BASE);

	ioclr32(cpcbase + reg, bits);
}

void mips_cpc_set(uint reg, ulong bits)
{
	void __iomem *cpcbase = __ioremap(CPC_BASE);

	ioset32(cpcbase + reg, bits);
}

////////////////////////////////////////////////////////////////////////////////
#include <barrier.h>
#include <lock.h>

static struct cpc_lock {
	struct lock	lock;
	ulong		flags;
} cpc_lock;

void mips_cpc_get(uint core)
{
	// take the lock first
	cpc_lock.flags = lock_acq_save(&cpc_lock.lock);

	// give access to the given core
	mips_cpc_cl_write(CPC_CX_OTHER, CPC_CX_OTHER_CORE(core));
	mb();
}

void mips_cpc_put(void)
{
	// OK, we can release the lock now
	lock_rel_rest(&cpc_lock.lock, cpc_lock.flags);
}
