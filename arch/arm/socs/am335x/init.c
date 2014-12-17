#include <init.h>
#include <iomap.h>
#include <utils/array-size.h>


static struct iomap_desc iomap_tbl[] = {
	{	// L4_PER
		.phys = 0x48000000,
		.size = 0x01000000,
		.virt = 0xF0000000,
	},
	{	// L4_FAST
		.phys = 0x4A000000,
		.size = 0x01000000,
		.virt = 0xF1000000,
	},
	{	// L4_WKUP
		.phys = 0x44C00000,
		.size = 0x00400000,
		.virt = 0xF2000000,
	},
};


static void am335x_init(void)
{
	printf("AM335x booting ...\n");

	iomap_init(iomap_tbl, 3);
}
core_initcall(am335x_init);
