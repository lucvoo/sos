#ifndef	_BITOPS_FINDBIT_H_
#define	_BITOPS_FINDBIT_H_

// Find Most Significant Bit
// Warning: the value is undefined for zero
// 0x00000001 => 0
// 0x8XXXXXXX => 31
static inline unsigned long bitop_fmsb(unsigned long val)
{
	return sizeof(val)*8 - 1 - __builtin_clzl(val);
}

#endif
