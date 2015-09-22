#ifndef	_GENERIC_UNALIGNED_OK_H_
#define	_GENERIC_UNALIGNED_OK_H_


static inline u16 read16_unaligned(const void *buf)
{
	return *((const u16 *)buf);
}

static inline u32 read32_unaligned(const void *buf)
{
	return *((const u32 *)buf);
}

#endif
