#include <kernel/bug.h>
#include <exceptions.h>


void __noreturn __bug(const struct eframe *regs, const char *file, const char *func, int line)
{
	printf("\nBUG!\n");
	dump_stack(regs, 0);

	// FIXME: stop the machine ?
	while (1)
		;
}
