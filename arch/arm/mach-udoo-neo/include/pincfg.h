#ifndef	_MACH_PINCFG_H_
#define	_MACH_PINCFG_H_

#include <hw/imx/pinctrl.h>

	// mux reg- val  |  cfg reg - val   | input reg - val     // PAD name		- mux

static const struct imx_pincfg imx_pincfg_enet1[] = {
	// ENET1 - RMII mode
	{ { 0x0084, 0x0 }, { 0x03CC, 0xa0b1 }, { 0x0,        } }, // ENET1_MDC		- ENET1_MDC
	{ { 0x0088, 0x0 }, { 0x03D0, 0xa0b1 }, { 0x0764, 0x1 } }, // ENET1_MDIO		- ENET1_MDIO
	{ { 0x0090, 0x1 }, { 0x03D8, 0x3081 }, { 0x0760, 0x1 } }, // ENET1_TX_CLK	- ENET1_REF_CLK1
	{ { 0x01C0, 0x0 }, { 0x0508, 0x3081 }, { 0x0,        } }, // RGMII1_RD0		- ENET1_RX_DATA_0
	{ { 0x01C4, 0x0 }, { 0x050C, 0x3081 }, { 0x0,        } }, // RGMII1_RD1		- ENET1_RX_DATA_1
	{ { 0x01D0, 0x0 }, { 0x0518, 0x3081 }, { 0x0,        } }, // RGMII1_RX_CTL	- ENET1_RX_EN
	{ { 0x01D4, 0x1 }, { 0x051C, 0x3081 }, { 0x0,        } }, // RGMII1_RXC		- ENET1_RX_ER
	{ { 0x01D8, 0x0 }, { 0x0520, 0xa0b1 }, { 0x0,        } }, // RGMII1_TD0		- ENET1_TX_DATA_0
	{ { 0x01DC, 0x0 }, { 0x0524, 0xa0b1 }, { 0x0,        } }, // RGMII1_TD1		- ENET1_TX_DATA_1
	{ { 0x01E8, 0x0 }, { 0x0530, 0xa0b1 }, { 0x0,        } }, // RGMII1_TX_CTL	- ENET1_TX_EN

	// phy clk (ENET_PHY_CLK)
	{ { 0x009C, 0x1 }, { 0x03E4, 0x0091 }, { 0x0,        } }, // ENET2_RX_CLK	- ENET2_REF_CLK_25M

	// phy IRQ (ENET_INT/ENET_IRQ)
	{ { 0x00A0, 0x5 }, { 0x03E8, 0x3081 }, { 0x0,        } }, // ENET2_TX_CLK	- GPIO2_IO09
};
#endif
