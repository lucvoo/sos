#ifndef	_BITS_H_
#define	_BITS_H_

#define	BIT(N)		(1U << (N))
#define	MSK(W)		(BIT(W) - 1U)
#define	MASK(O, W)	(MSK(W) << (O))

#define	BITS(O, W)	(O), (W)

#define	BITS_EXTRACT(VAL, BITS)	_BIT_EXTRACT(VAL, BITS)
#define	_BIT_EXTRACT(VAL, O, W)	(((VAL) >> (O)) & MSK(W))
#define	BITS_INSERT(BASE, BITS, VAL)	_BIT_INSERT(BASE, BITS, VAL)
#define	_BIT_INSERT(BASE, O, W, VAL)	((BASE & ~MASK(O, W)) | ((VAL) << (O)))

#define	BITS_WIDTH(BITS)		_BITS_WIDTH(BITS)
#define	_BITS_WIDTH(O, W)		(W)
#define	BITS_OFFSET(BITS)		_BITS_OFFSET(BITS)
#define	_BITS_OFFSET(O, W)		(O)

#endif
