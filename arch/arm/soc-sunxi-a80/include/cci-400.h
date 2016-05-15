#ifndef	_SOC_CCI_400_H_
#define	_SOC_CCI_400_H_

#include <arch/cci-400.h>

#define	CCI400_BASE	0x01c90000	// this include the 0x90000 offset!


static inline int cci400_interface(uint cluster_id)
{
	return cluster_id + 3;
}

#endif
