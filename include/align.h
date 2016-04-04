#ifndef	_ALIGN_H_
#define	_ALIGN_H_


// A must be a power of 2! Check that!
#define	ALIGN_MASK(X, A)	((typeof(X)) ((A) - 1))
#define	ALIGN(X, A)		((ALIGN_MASK(X, X)|ALIGN_MASK(X, A)) + 1)

#endif
