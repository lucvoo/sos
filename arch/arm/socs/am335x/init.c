#include <init.h>


static void am335x_init(void)
{
	printf("AM335x booting ...\n");
}
core_initcall(am335x_init);
