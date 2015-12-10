#ifndef	_SOC_IOMUX_H_
#define	_SOC_IOMUX_H_

#include <hw/imx/pinctrl.h>


void imx_iomux_config(const struct imx_pincfg *cfg, uint nbr);

#define	imx_iomux(CFG)	imx_iomux_config(CFG, ARRAY_SIZE(CFG))

#endif
