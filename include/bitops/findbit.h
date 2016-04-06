#ifndef	_BITOPS_FINDBIT_H_
#define	_BITOPS_FINDBIT_H_

/**
 * Find First (bit) Set
 *
 * 0		-> 0
 * XXXXXXX1	-> 1
 * XXXXXX10	-> 2
 * ...
 */
#define	bitop_ffs(val)	__builtin_ffsl(val)

/**
 * Count Trailing Zeroes
 *
 * 0		-> undef
 * XXXXXXX1	-> 0
 * XXXXXX10	-> 1
 * ...
 */
#define	bitop_ctz(val)	__builtin_ctzl(val)

/**
 * Count Leading Zeroes
 *
 * 1XXXXXXX	-> 0
 * 01XXXXXX	-> 1
 * ...
 * 00000001	-> 31
 * 00000000	-> undef
 *         	-> 32 on ARM since ARMv5T
 *         	-> 32 on MIPS since forever
 *         	-> 32 on PPC (since forever ?)
 *         	-> 32 on x86 (if -mlzcnt is given and supported)
 */
#define	bitop_clz(val)	__builtin_clzl(val)

/**
 * Find Most Significant Bit
 *
 * 00000000	-> undef when clz is undef, -1 when clz is well behaved
 * 00000001	-> 0
 * 0000001X	-> 1
 * ...
 */
static inline unsigned long bitop_fmsb(unsigned long val)
{
	return sizeof(val)*8 - 1 - __builtin_clzl(val);
}

/**
 * Find (upper) binary logarithm
 *
 * 00000000	-> undef
 * 00000001	-> 0 (if clz(0) is well bahaved)
 * 00000010	-> 1
 * 00000011	-> 2
 * 00000100	-> 2
 * 00000101	-> 3
 * ...
 * 00000111	-> 3
 * 00001000	-> 3
 * 00001XXX	-> 4
 * ...
 */
static inline unsigned long bitop_log2(unsigned long val)
{
	return sizeof(val)*8 - __builtin_clzl(val - 1);
}

#endif
