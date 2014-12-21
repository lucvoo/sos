#ifndef	_ROUND_H_
#define	_ROUND_H_


#define	DIV_ROUND_UP(X, A)	(((X) + (A) - 1)/(A))

// A must be a power of 2! Can we check that?
#define	ALIGN_MASK(X, A)	((typeof(X)) ((A) - 1))
#define	ALIGN(X, A)		((ALIGN_MASK(X, X)|ALIGN_MASK(X, A)) + 1)

#endif
