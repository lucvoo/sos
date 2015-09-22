#ifndef	_GENERIC_UNALIGNED_LE_H_
#define	_GENERIC_UNALIGNED_LE_H_


static inline u16 read16_unaligned(const void *buf)
{
	const u8 *b = buf;
	return b[1] << 8 | b[0];
}

static inline u32 read32_unaligned(const void *buf)
{
	const u8 *b = buf;
	return b[3] << 24 | b[2] << 16 | b[1] << 8 | b[0];
}

#endif
