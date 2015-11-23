#ifndef	_BITS_H_
#define	_BITS_H_

#define	BIT(N)		(1U << (N))
#define	MSK(W)		(BIT(W) - 1U)
#define	MASK(O, W)	(MSK(W) << (O))

#define	BITS(O, W)	(O), (W)

#define	BITS_EXTRACT(VAL, BITS)	_BIT_EXTRACT(VAL, BITS)
#define	_BIT_EXTRACT(VAL, O, W)	(((VAL) >> (O)) & MSK(W))
#define	BITS_INSERT(VAL, BITS)	_BIT_INSERT(VAL, BITS)
#define	_BIT_INSERT(VAL, O, W)	((VAL) << (O))

#endif
