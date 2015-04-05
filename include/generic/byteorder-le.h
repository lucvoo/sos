#ifndef	_GENERIC_BYTEORDER_LE_H_
#define	_GENERIC_BYTEORDER_LE_H_

#include <swab.h>
#include <types/kint.h>

#define	be16_to_cpu(X)		__swab16(X)
#define	be32_to_cpu(X)		__swab32(X)
#define	be64_to_cpu(X)		__swab64(X)
#define	cpu_to_be16(X)		__swab16(X)
#define	cpu_to_be32(X)		__swab32(X)
#define	cpu_to_be64(X)		__swab64(X)

#define	le16_to_cpu(X)		((u16)(X))
#define	le32_to_cpu(X)		((u32)(X))
#define	le64_to_cpu(X)		((u64)(X))
#define	cpu_to_le16(X)		((u16)(X))
#define	cpu_to_le32(X)		((u32)(X))
#define	cpu_to_le64(X)		((u64)(X))

#endif
