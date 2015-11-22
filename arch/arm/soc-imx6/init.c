#include <init.h>
#include <iomap.h>
#include <hw/am335x.h>
#include <utils/array-size.h>


static struct iomap_desc iomap_tbl[] = {
	{	// AIPS 1-3
		.phys = 0x02000000,
		.size = 0x00300000,
		.virt = 0xFF000000,
	},
	{	// ARM MP + PL310
		.phys = 0x00A00000,
		.size = 0x00100000,
		.virt = 0xFF300000,
	},
};


static void imx6_init(void)
{
	iomap_init(iomap_tbl, ARRAY_SIZE(iomap_tbl));
}
core_initcall(imx6_init);
