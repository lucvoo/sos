#include <init.h>


static void __init early_arch_init(void)
{
	// TODO: DT ot ATAG parsing
}
early_initcall(early_arch_init);
