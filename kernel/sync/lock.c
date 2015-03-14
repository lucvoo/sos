#include <lock.h>


void _lock_acq(struct lock *lock)
{
	__lock_acq(lock);
}

void _lock_rel(struct lock *lock)
{
	__lock_rel(lock);
}
