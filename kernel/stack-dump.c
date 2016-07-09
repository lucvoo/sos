#include <exceptions.h>
#include <lock.h>
#include <smp.h>


void dump_stack(const struct eframe *f, uint flags)
{
	extern struct lock printf_lock;
	ulong status = lock_acq_save(&printf_lock);

	__printf("\nSTACK DUMP:");

#ifdef	CONFIG_SMP
	__printf(" (CPU %d)", __coreid());
#endif
	__printf("\n");

	__arch_dump_stack(f);

	lock_rel_rest(&printf_lock, status);
}
