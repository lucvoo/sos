#ifndef	_HW_IMX_PINCTRL_H_
#define	_HW_IMX_PINCTRL_H_

#define PAD_CTL_SRE_SLOW	(0 << 0)
#define PAD_CTL_SRE_FAST	(1 << 0)
#define PAD_CTL_DSE_HIZ		(0 << 3)
#define PAD_CTL_DSE_260		(1 << 3)
#define PAD_CTL_DSE_130		(2 << 3)
#define PAD_CTL_DSE_87		(3 << 3)
#define PAD_CTL_DSE_65		(4 << 3)
#define PAD_CTL_DSE_52		(5 << 3)
#define PAD_CTL_DSE_43		(6 << 3)
#define PAD_CTL_DSE_37		(7 << 3)
#define PAD_CTL_SPEED_LOW	(0 << 6)
#define PAD_CTL_SPEED_MED	(1 << 6)
#define PAD_CTL_SPEED_MAX	(3 << 6)
#define PAD_CTL_ODT_DISABLE	(0 << 8)
#define PAD_CTL_ODT_120		(1 << 8)
#define PAD_CTL_ODT_60		(2 << 8)
#define PAD_CTL_ODT_40		(3 << 8)
#define PAD_CTL_ODT_30		(4 << 8)
#define PAD_CTL_ODT_24		(5 << 8)
#define PAD_CTL_ODT_20		(6 << 8)
#define PAD_CTL_ODT_17		(7 << 8)
#define PAD_CTL_ODE		(1 << 11)
#define PAD_CTL_PKE		(1 << 12)
#define PAD_CTL_PUE		(1 << 13)
#define PAD_CTL_PD_100K		(0 << 14)
#define PAD_CTL_PU_47K		(1 << 14)
#define PAD_CTL_PU_100K		(2 << 14)
#define PAD_CTL_PU_22K		(3 << 14)
#define PAD_CTL_HYS		(1 << 16)

struct pincfg_reg_val {
	u16	reg;
	u16	val;
};

struct imx_pincfg {
	struct pincfg_reg_val	mux;
	struct pincfg_reg_val	config;
	struct pincfg_reg_val	input;
};

#endif
