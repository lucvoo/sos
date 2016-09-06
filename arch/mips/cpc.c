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
