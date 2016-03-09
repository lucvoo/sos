#include <init.h>


static void ar933x_init(void)
{
	printf("AR933x booting ...\n");
}
core_initcall(ar933x_init);
