#ifndef	_GENERIC_UNALIGNED_BE_H_
#define	_GENERIC_UNALIGNED_BE_H_


static inline u16 read16_unaligned(const void *buf)
{
	const u8 *b = buf;
	return b[0] << 8 | b[1];
}

static inline u32 read32_unaligned(const void *buf)
{
	const u8 *b = buf;
	return b[0] << 24 | b[1] << 16 | b[2] << 8 | b[3];
}

#endif
