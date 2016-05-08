#include <init.h>
#include <iomap.h>


static struct iomap_desc iomap_tbl[] = {
#if 0
	{	// BROM & SRAM: needed finer grained prot
		.phys = 0x00000000,
		.size = 0x00100000,
		.virt = 0xFE000000,
	},
#endif
	{	// AHB1
		.phys = 0x00800000,
		.size = 0x00300000,
		.virt = 0xFE100000,
	},
	{	// CoreSight, TSGEN, CPU_CFG & TIMESTAMP
		.phys = 0x01400000,
		.size = 0x00400000,
		.virt = 0xFE400000,
	},
	{	// AHB0
		.phys = 0x01C00000,
		.size = 0x00100000,
		.virt = 0xFE800000,
	},
#if 0
	{	// SRAM A4
		.phys = 0x01D00000,
		.size = 0x00100000,
		.virt = 0xFE900000,
	},
#endif
	{	// AHB2
		.phys = 0x03000000,
		.size = 0x01000000,
		.virt = 0xFEA00000,
	},
	{	// APB0
		.phys = 0x06000000,
		.size = 0x00100000,
		.virt = 0xFFA00000,
	},
	{	// APB1
		.phys = 0x07000000,
		.size = 0x00100000,
		.virt = 0xFFB00000,
	},
	{	// CPUS
		.phys = 0x08000000,
		.size = 0x00100000,
		.virt = 0xFFC00000,
	},
#if 0
	{	// SRAM A2
		.phys = 0x08100000,
		.size = 0x00100000,
		.virt = 0xFFD00000,
	},
#endif
};


static void sunxi_a80_init(void)
{
	iomap_init(iomap_tbl, ARRAY_SIZE(iomap_tbl));
}
core_initcall(sunxi_a80_init);
