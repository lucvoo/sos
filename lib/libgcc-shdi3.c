#include <symbols.h>
#include <libgcc.h>
#include <64bit.h>


s64 __weak __ashldi3(s64 v, uint n)
{
	u2x32_t w = { .hl = v, };
	u2x32_t r;

	if (n == 0)
		return v;

	if (n >= 32) {
		r.hi = (u32) w.lo << (n - 32);
		r.lo = (u32) 0;
	} else {
		r.hi = (u32) w.hi << n | (u32) w.lo >> (32 - n);
		r.lo = (u32) w.lo << n;
	}

	return r.hl;
}
strong_alias(__ashldi3, __aeabi_llsl);

s64 __weak __ashrdi3(s64 v, uint n)
{
	u2x32_t w = { .hl = v, };
	u2x32_t r;

	if (n == 0)
		return v;

	if (n >= 32) {
		r.hi = (s32) w.hi >> 31;
		r.lo = (s32) w.hi >> (n - 32);
	} else {
		r.hi = (s32) w.hi >> n;
		r.lo = (u32) w.lo >> n | (u32) w.hi << (32 - n);
	}

	return r.hl;
}
strong_alias(__ashrdi3, __aeabi_lasr);

u64 __weak __lshrdi3(u64 v, uint n)
{
	u2x32_t w = { .hl = v, };
	u2x32_t r;

	if (n == 0)
		return v;

	if (n >= 32) {
		r.hi = (u32) 0;
		r.lo = (u32) w.hi >> (n - 32);
	} else {
		r.hi = (u32) w.hi >> n;
		r.lo = (u32) w.lo >> n | (u32) w.hi << (32 - n);
	}

	return r.hl;
}
strong_alias(__lshrdi3, __aeabi_llsr);
