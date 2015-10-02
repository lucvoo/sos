#ifndef	_UNALIGNED_H_
#define	_UNALIGNED_H_

#include <generic/unaligned.h>
#include <byteorder.h>

#define	readle16(BUF)	le16_to_cpu(read16_unaligned(BUF))
#define	readle32(BUF)	le32_to_cpu(read32_unaligned(BUF))

#endif
