#include <init.h>
#include <iomap.h>


static void jz4780_init(void)
{
	printf("JZ4780 booting ...\n");
}
core_initcall(jz4780_init);
