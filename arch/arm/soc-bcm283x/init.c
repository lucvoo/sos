#include <init.h>
#include <iomap.h>


static struct iomap_desc iomap_tbl[] = {
	{
		.phys = 0x20000000,
		.size = 0x01000000,
		.virt = 0xfe000000,
	},
};


static void __init bcm2835_init(void)
{
	printf("bcm2835x booting ...\n");

	iomap_init(iomap_tbl, 1);
}
core_initcall(bcm2835_init);
