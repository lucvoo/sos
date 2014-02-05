#include <exceptions.h>


void und_handler(const struct eframe *regs)
{
	// FIXME
	printf("\n!!! Exception: undefined instruction\n");
	dump_stack(regs, 0);
}
