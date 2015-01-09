#ifndef	_UTILS_ARRAY_ITER_H_
#define	_UTILS_ARRAY_ITER_H_

#include <utils/array-size.h>

#define	for_each_array_item(p, array) \
	for (p = &(array)[0]; p < &(array)[ARRAY_SIZE(array)]; p++)

#endif
