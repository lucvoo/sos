#ifndef	_SYMBOLS_H_
#define	_SYMBOLS_H_

#define	__alias(NAME)	__attribute__((alias(#NAME)))
#define	__weak		__attribute__((weak))

#define	__def_alias(NAME, ALIAS)	__typeof__(NAME) ALIAS __alias(NAME)

/**
	Define weak a strong aliases.
	They are meant to be used like this:
	void function_generic(void)
	{
		// Generic implementation.
		// Preferably be replaced by a specific one
		...
	}
	weak_alias(function_generic, function);
*/
#define weak_alias(NAME, ALIAS)		extern __def_alias(NAME, ALIAS) __weak
#define strong_alias(NAME, ALIAS)	extern __def_alias(NAME, ALIAS)

#endif
