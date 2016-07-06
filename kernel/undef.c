#include <exceptions.h>
#include <halt.h>


void und_handler(const struct eframe *regs)
{
	// FIXME
	printf("\n!!! Exception: undefined instruction\n");
	__halt(regs);
}
