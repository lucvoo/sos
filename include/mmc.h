#ifndef	_MMC_H_
#define	_MMC_H_


struct mmc_cmd {
	u32		cmd;
	u32		arg;
	u32		resp[4];

	u16		bnbr;
	u16		bsiz;
	union {
		void		*rbuff;
		const void	*wbuff;
	};
};

struct mmc_host {
	u32	mode;
#define	MMC_MODE_SD	0x200
#define	MMC_MODE_SD2		(MMC_MODE_SD | 0x20)
#define	MMC_MODE_MMC	0x100

	u32	vdds;
#define	MMC_VDD_36_35		(1 << 23)
#define	MMC_VDD_35_34		(1 << 22)
#define	MMC_VDD_34_33		(1 << 21)
#define	MMC_VDD_33_32		(1 << 20)
#define	MMC_VDD_32_31		(1 << 19)
#define	MMC_VDD_31_30		(1 << 18)
#define	MMC_VDD_30_29		(1 << 17)
#define	MMC_VDD_29_28		(1 << 16)
#define	MMC_VDD_28_27		(1 << 15)
#define	MMC_VDD(MAX, MIN)	(2 * MMC_VDD_ ## MAX - MMC_VDD_ ## MIN)
#define	MMC_VDD_MASK		MMC_VDD(36_35, 28_27)

	u32	caps;
#define	MMC_CAP_4BIT		(1 << 0)	// can do 4 bit transfers
#define	MMC_CAP_8BIT		(1 << 1)	// can do 8 bit transfers
#define	MMC_CAP_SDHC		(1 << 2)	// support SDHC
#define	MMC_CAP_SDXC		(1 << 3)	// support SDXC
#define	MMC_CAP_18V		(1 << 4)	// support DDR mode at 1.8V
#define	MMC_CAP_HS_MMC		(1 << 5)	// support MMC high-speed
#define	MMC_CAP_HS_SD		(1 << 6)	// SUPPORT SD high-speed
#define	MMC_CAP_AUTO_CMD23	(1 << 7)	// can send CMD23 automatically
#define	MMC_CAP_AUTO_CMD12	(1 << 8)	// can send CMD12 automatically

	u8	bus_width;

	uint	f_min;
	uint	f_max;
	uint	freq;
	u32	ocr;
	u32	scr;
	u32	rca;	// RCA << 16
	u32	capacity;	// in 512bit sectors (OK till 2TB);

	const char *name;

	int (*reset)(struct mmc_host *host);
	int (*get_cd)(struct mmc_host *host);
	int (*set_bus_width)(struct mmc_host *host, uint width);
	int (*set_freq)(struct mmc_host *host, uint freq);
	int (*send_cmd)(struct mmc_host *host, struct mmc_cmd *);
};

#endif
