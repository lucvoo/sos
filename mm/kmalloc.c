#include <kmalloc.h>
#include <page-alloc.h>
#include <atomic.h>
#include <round.h>
#include <page.h>
#include <memory.h>
#include <lock.h>


static struct kmalloc_perm_state {
	struct lock	lock;
	void		*buf;
	unsigned int	left;
} kmalloc_perm_state;

static void *kmalloc_perm(unsigned int size, unsigned int aflags)
{
	struct kmalloc_perm_state *state = &kmalloc_perm_state;
	unsigned long flags;
	void *ptr;

	if (size > PAGE_SIZE/4)
		return NULL;

	flags = lock_acq_save(&state->lock);

	if (size > state->left) {
		struct page *p;

		p = page_alloc(0, aflags);
		if (!p) {
			ptr = NULL;
			goto end;
		}

		atomic_or(&p->flags, PG_perm);
		state->buf = page_to_virt(p);
		state->left = PAGE_SIZE;
	}

	ptr = state->buf;
	state->buf += size;
	state->left -= size;

end:
	lock_rel_rest(&state->lock, flags);
	return ptr;
}


void *kmalloc(unsigned int size, unsigned int aflags)
{
	void *ptr;

	size = ALIGN(size, sizeof(ulong));

	if (aflags & GFP_PERM)
		ptr = kmalloc_perm(size, aflags);
	else
		ptr = NULL;

	if (ptr && (aflags & GFP_ZERO))
		memset(ptr, 0, size);

	return ptr;
}
