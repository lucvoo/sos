#include <exceptions.h>
#include <lock.h>


void dump_stack(const struct eframe *f, uint flags)
{
	extern struct lock printf_lock;
	ulong status = lock_acq_save(&printf_lock);

	__arch_dump_stack(f);

	lock_rel_rest(&printf_lock, status);
}
