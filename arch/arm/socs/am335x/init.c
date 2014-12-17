#include <init.h>
#include <iomap.h>
#include <hw/am335x.h>
#include <utils/array-size.h>


static struct iomap_desc iomap_tbl[] = {
	{	// L4_PER
		.phys = L4_PER_PHYS,
		.size = L4_PER_SIZE,
		.virt = L4_PER_VIRT,
	},
	{	// L4_FAST
		.phys = L4_FAST_PHYS,
		.size = L4_FAST_SIZE,
		.virt = L4_FAST_VIRT,
	},
	{	// L4_WKUP
		.phys = L4_WKUP_PHYS,
		.size = L4_WKUP_SIZE,
		.virt = L4_WKUP_VIRT,
	},
};


static void am335x_init(void)
{
	printf("AM335x booting ...\n");

	iomap_init(iomap_tbl, 3);
}
core_initcall(am335x_init);
