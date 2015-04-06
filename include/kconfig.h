#ifndef	__KCONFIG_H_
#define	__KCONFIG_H_

/*
 * The original prototype of how to implement a C and CPP compatible
 * way of doing this came from the Google+ user "comex" in response to
 * Linus' crowdsourcing challenge for a possible improvement on his
 * earlier, C only, solution:
 *
 *	#define config_enabled(x)	(__stringify(x)[0] == '1')
 *
 * Shamelessly taken from Linux's kernel, of course.
 *
 * Tricky!
 * This use the fact that if a symbol 'X' is defined (to '1')
 * __comma##X will expand to '__comma1' which we define to something
 * containing a comma wich itself will later create a new argument.
 * If 'X' is not defined (or defined to something else than '1')
 * this won't happen and the macro will have one argument less.
 * This difference in the number of arguments is then used to
 * expand to '0' or '1'.
 */

#define	__comma1	,			//
#define	__kconfig1(X)	__kconfig2(__comma##X)	// X is now '1' or ''
#define	__kconfig2(Z)	__kconfig3(Z 1, 0)	// Z is now '1,' or '__comma...'
#define	__kconfig3(Z, X, ...)	X		// X is now ''1' or '0'

#define	kconfig(X)	__kconfig1(X)

#endif
