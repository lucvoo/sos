#include <kernel/bug.h>
#include <exceptions.h>
#include <halt.h>


void __noreturn __bug(const struct eframe *regs, const char *file, const char *func, int line)
{
	printf("\nBUG!\n");

	__halt(regs);
}
