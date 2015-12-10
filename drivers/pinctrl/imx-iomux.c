#include <hw/imx/pinctrl.h>
#include <soc/iomux.h>
#include <init.h>
#include <io.h>


struct iomuxc {
	void __iomem *iobase;
};

static void iomux_config_reg(struct iomuxc *iomuxc, const struct pincfg_reg_val *rv, int null_test)
{
	if (null_test && rv->reg == 0)
		return;

	iowrite32(iomuxc->iobase + rv->reg, rv->val);
}

static void iomux_config_pin(struct iomuxc *iomuxc, const struct imx_pincfg *cfg)
{
	iomux_config_reg(iomuxc, &cfg->mux, 0);
	iomux_config_reg(iomuxc, &cfg->config, 1);
	iomux_config_reg(iomuxc, &cfg->input, 1);
}

static void iomux_config_group(struct iomuxc *iomuxc, const struct imx_pincfg *cfg, uint nbr)
{
	uint i;

	for (i = 0; i < nbr; i++, cfg++)
		iomux_config_pin(iomuxc, cfg);
}


static struct iomuxc iomuxc;

void imx_iomux_config(const struct imx_pincfg *cfg, uint nbr)
{
	iomux_config_group(&iomuxc, cfg, nbr);
}

#include <mach/pincfg.h>
static void imx_pincfg_init(void)
{
#define	IOMUXC_BASE	0x020E0000
#define	IOMUXC_SIZE	    0x4000
	iomuxc.iobase = ioremap(IOMUXC_BASE, IOMUXC_SIZE);
}
board_setup_initcall(imx_pincfg_init);
