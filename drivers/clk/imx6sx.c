#include "clk-divider.h"
#include "clk-divtable.h"
#include "clk-fixed.h"
#include "clk-ratio.h"
#include "clk-mask.h"
#include "clk-mux-mmio.h"
#include "clk-mmio-ratio.h"
#include "clk-mmio.h"
#include "imx6-pll.h"
#include "imx-pfd.h"
#include <soc/ccm.h>
#include <init.h>
#include <io.h>


static struct clk_fixed		ckil;
static struct clk_fixed		osc;
static struct clk_imx6pll	pll2;
static struct clk_imxpfd	pll2_pfd2;
static struct clk_imx6pll	pll6;
static struct clk_mmio		pll6_enet;
static struct clk_mux_mmio	pll6_bypass;
static struct clk * const pll6_bypass_parents[2] = {
	[0] = &pll6.clk,
};
static struct clk_divider	ahb;
static struct clk_divider	ipg;
static struct clk_mask		enet;
static struct clk_mask		enet_ahb;
static struct clk_divider	enet_podf;
static struct clk_mux_mmio	enet_sel;
static struct clk * const enet_sel_parents[8] = {
	[0] = &enet_podf.clk,
};
static struct clk_mux_mmio	enet_pre_sel;
static struct clk * const enet_pre_sel_parents[8] = {
	[0] = &pll2.clk,
	[2] = &osc.clk,
	[4] = &pll2_pfd2.clk,
};
static struct clk_divtable	enet_ref;
static const struct divtable	enet_ref_tbl[] = {
	{ .val = 0,	.div = 20, },
	{ .val = 1,	.div = 10, },
	{ .val = 2,	.div = 5, },
	{ .val = 3,	.div = 4, },
	{		.div = 0, },
};
static struct clk_mmio_ratio	enet_ptp;


static void imx6sx_clock_setup(void)
{
	void __iomem *ccmbase;

	ccmbase = ioremap(CCM_BASE, CCM_SIZE);

	// root clocks
	clk_fixed_init(&ckil, "ckil", 1 << 15);
	clk_fixed_init(&osc,  "osc",   24000000);

	// secondary clocks
	pll2.clk.name = "pll2";
	pll2.clk.parent = &osc.clk;
	pll2.mask = 0x1;
	clk_imx6pll_register(&pll2, ccmbase, IMX6PLLT_SYS);

	pll2_pfd2.clk.name = "pll2_pfd2";
	pll2_pfd2.clk.parent = &pll2.clk;
	pll2_pfd2.reg = ccmbase + CCM_PLL_SYS_PFD;
	pll2_pfd2.pfd = 2;
	clk_imxpfd_register(&pll2_pfd2);

	pll6.clk.name = "pll6";
	clk_imx6pll_register(&pll6, ccmbase, IMX6PLLT_ENET);

	pll6_bypass.clk.name = "pll6_bypass";
	pll6_bypass.reg = ccmbase + CCM_PLL_ENET;
	pll6_bypass.shift = 16;
	pll6_bypass.width = 1;
	pll6_bypass.parents = pll6_bypass_parents;
	clk_mux_mmio_register(&pll6_bypass, &pll6.clk);

#define	pll6_bypass pll6

	pll6_enet.clk.name = "pll6_enet";
	pll6_enet.clk.parent = &pll6_bypass.clk;
	pll6_enet.gate.reg = ccmbase + CCM_PLL_ENET;
	pll6_enet.gate.bit = 13;
	clk_mmio_register(&pll6_enet);

	// Bus clocks
	ahb.clk.name = "ahb";
	ahb.clk.parent = &pll2_pfd2.clk;	// FIXME: muxed
	ahb.iobase = ccmbase + CCM_CBCDR;
	ahb.scale = 0;
	ahb.offset = 1;
	ahb.width = 3;
	ahb.shift = 10;
	clk_divider_register(&ahb);

	ipg.clk.name = "ipg";
	ipg.clk.parent = &ahb.clk;
	ipg.iobase = ccmbase + CCM_CBCDR;
	ipg.scale = 0;
	ipg.offset = 1;
	ipg.width = 2;
	ipg.shift = 8;
	clk_divider_register(&ipg);

	// ethernet
	enet.clk.name = "enet";
	enet.clk.parent = &ipg.clk;
	enet.gate.reg = ccmbase + CCM_CCGR3;
	enet.gate.msk= 0x30;
	clk_mask_register(&enet);

	enet_pre_sel.clk.name = "enet_pre_sel";
	enet_pre_sel.reg = ccmbase + CCM_CHSCCDR;
	enet_pre_sel.shift = 15;
	enet_pre_sel.width = 3;
	enet_pre_sel.parents = enet_pre_sel_parents;
	clk_mux_mmio_register(&enet_pre_sel, &pll2_pfd2.clk);

	enet_podf.clk.name = "enet_podf";
	enet_podf.clk.parent = &enet_pre_sel.clk;
	enet_podf.iobase = ccmbase + CCM_CHSCCDR;
	enet_podf.scale = 0;
	enet_podf.offset = 1;
	enet_podf.shift = 12;
	enet_podf.width = 3;
	clk_divider_register(&enet_podf);

	enet_sel.clk.name = "enet_sel";
	enet_sel.reg = ccmbase + CCM_CHSCCDR;
	enet_sel.shift = 9;
	enet_sel.width = 3;
	enet_sel.parents = enet_sel_parents;
	clk_mux_mmio_register(&enet_sel, &enet_podf.clk);

	enet_ahb.clk.name = "enet_ahb";
	enet_ahb.clk.parent = &enet_sel.clk;
	enet_ahb.gate.reg = ccmbase + CCM_CCGR3;
	enet_ahb.gate.msk= 0x30;
	clk_mask_register(&enet_ahb);

	enet_ref.clk.name = "enet_ref";
	enet_ref.clk.parent = &pll6_enet.clk;
	enet_ref.iobase = ccmbase + CCM_PLL_ENET;
	enet_ref.tbl = enet_ref_tbl;
	enet_ref.shift = 0;
	enet_ref.width = 2;
	clk_divtable_register(&enet_ref);
	clk_set_rate(&enet_ref.clk, 125000000);

	enet_ptp.clk.name = "enet_ptp";
	enet_ptp.clk.parent = &pll6_enet.clk;
	enet_ptp.gate.reg = ccmbase + CCM_PLL_ENET;
	enet_ptp.gate.bit = 21;
	enet_ptp.rate.mult = 1;
	enet_ptp.rate.div = 20;
	clk_mmio_ratio_register(&enet_ptp);
}
board_setup_initcall(imx6sx_clock_setup);
