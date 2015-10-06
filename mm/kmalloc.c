#include "kmalloc-internal.h"
#include <kmalloc.h>
#include <page-alloc.h>
#include <atomic.h>
#include <round.h>
#include <page.h>
#include <memory.h>
#include <lock.h>
#include <string.h>


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

	size = ALIGN(size, sizeof(unsigned long));

	if (aflags & GFP_PERM)
		ptr = kmalloc_perm(size, aflags);
	else
		ptr = __kmalloc(size, aflags);

	if (ptr && (aflags & GFP_ZERO))
		memset(ptr, 0, size);

	return ptr;
}

void *kzalloc(unsigned int size, unsigned int aflags)
{
	return kmalloc(size, aflags | GFP_ZERO);
}

void *krealloc(void *optr, uint nsize, uint aflags)
{
	uint osize;
	uint csize;
	void *nptr;

	if (!optr)
		return kmalloc(nsize, aflags);

	nsize = ALIGN(nsize, sizeof(ulong));
	osize = __kmalloc_size(optr);

	// check if we need to expand, shrink or leave the buffer as it is
	if (osize < nsize)
		csize = osize;
	else if (2 * (osize - nsize) > nsize)
		csize = nsize;
	else
		return (void *) optr;

	nptr = kmalloc(nsize, aflags & ~GFP_ZERO);
	if (!nptr)
		goto fail;

	memcpy(nptr, optr, csize);
	if ((aflags & GFP_ZERO) && (nsize > csize))
		memset(nptr + csize, 0, nsize - csize);

fail:
	kfree(optr);
	return nptr;
}
