#include <arch/iomap.h>
#include <arch/copro.h>
#include <arch/cm.h>
#include <init.h>
#include <io.h>


static void __iomem *gcrbase;

ulong mips_gcr_read(uint reg)
{
	return ioread32(gcrbase + reg);
}

void mips_gcr_write(uint reg, ulong val)
{
	iowrite32(gcrbase + reg, val);
}

void mips_gcr_clr(uint reg, ulong bits)
{
	ioclr32(gcrbase + reg, bits);
}

void mips_gcr_set(uint reg, ulong bits)
{
	ioset32(gcrbase + reg, bits);
}

static void gcrbase_init(void)
{
	ulong pbase;
	u32 cfg3;

	cfg3 = c0_getval(c0_config3);
	if ((cfg3 & CFG3_CMGCR) == 0)
		return;

	pbase = c0_getval(c0_cmgcrbase) << 4;
	gcrbase = __ioremap(pbase);

	// enable Global Interrupt Control
	iowrite32(gcrbase + GCR_GIC_BASE, GIC_BASE | GCR_GIC_BASE_EN);

	// enable Cluster Power Control
	iowrite32(gcrbase + GCR_CPC_BASE, CPC_BASE | GCR_CPC_BASE_EN);
}

static void __init mips_cm_init(void)
{
	gcrbase_init();
}
pure_initcall(mips_cm_init);
