#ifndef	_SOC_MODEL_H_

#if	defined(CONFIG_STM32F10X)
#include <soc/stm32f10x.h>
#elif	defined(CONFIG_STM32F4XX)
#include <soc/clock-config.h>
#include <soc/stm32f4xx.h>
#endif

#endif
