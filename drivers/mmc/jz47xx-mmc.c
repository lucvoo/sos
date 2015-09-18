/*
 * Ingenic JZ MMC driver
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Support:
 *	MMC  v4.2
 *	SD   v3.0 (UHS-I is SD v3.01...)
 *	SDIO v1.0 (with 1 cmd channnel & 4 data channels)
 */

#include <mmc.h>
#include <mmc/cmds.h>
#include <mmc/drivers.h>
#include <io.h>
#include <delay.h>
#include <round.h>
#include <errno.h>
#include <clk.h>
#include <soc/msc.h>



#define JZ_MMC_CLK_RATE 24000000

struct jzmmc {
	struct mmc_host	host;

	void __iomem	*iobase;
	struct clk	*clk;
	uint		flags;
#define JZ_MMC_BUS_WIDTH_MASK 0x3
#define JZ_MMC_BUS_WIDTH_1    0x0
#define JZ_MMC_BUS_WIDTH_4    0x2
#define JZ_MMC_BUS_WIDTH_8    0x3
#define JZ_MMC_SENT_INIT (1 << 2)
};


static int jzmmc_get_cd(struct mmc_host *host)
{
	return 1;	// FIXME
}

static uint jzmmc_poll_status(struct jzmmc *jz, unsigned int mask)
{
	int tries = 1000;
	uint stat;

	do {
		stat  = ioread32(jz->iobase + MSC_STAT);

		// FIXME: if ! card present ...
		//	return -ENOMEDIUM

		if (stat & STAT_ERR)
			break;

		if (stat & mask)
			break;

		udelay(1000);
	} while (tries--);

	return stat;
}

static int jzmmc_poll_irq(struct jzmmc *jz, unsigned int mask)
{
	uint tries = 0x1000;

	while (--tries) {
		u32 iflgs;

		iflgs = ioread32(jz->iobase + MSC_IFLG);
		if (iflgs & mask)
			return 0;

		udelay(1000);		// FIXME
	}

	return -ETIMEDOUT;
}

static int jzmmc_reset(struct mmc_host *host)
{
	struct jzmmc *jz = container_of(host, struct jzmmc, host);

	// reset
	iowrite32(jz->iobase + MSC_CTRL, CTRL_RESET);
	while (ioread32(jz->iobase + MSC_STAT) & STAT_IS_RESETTING)
		;

	// maximum timeouts
	iowrite32(jz->iobase + MSC_RESTO, 0xffff);
	iowrite32(jz->iobase + MSC_RDTO, 0xffffff);

	// enable low power mode
	iowrite32(jz->iobase + MSC_LPM, LPM_LPM);

	// mask & clear interrupts
	iowrite32(jz->iobase + MSC_IMASK, 0xffffffff);
	iowrite32(jz->iobase + MSC_IFLG,  0xffffffff);

	return 0;
}

static int jzmmc_read_data(struct jzmmc *jz, struct mmc_data *data)
{
	uint nbr = DIV_ROUND_UP(data->blk_nbr * data->blk_size, 4);
	u32 *buf = data->rbuff;

	while (nbr > 0) {
		uint cnt;

		if (jzmmc_poll_irq(jz, INT_RXFIFO_RD_REQ))
			goto timeout;

		// How many words is in the FIFO?
		cnt = ioread32(jz->iobase + MSC_RTCNT);
		if (cnt == 0) {
			pr_dbg("RTCNT is zero while requested data present\n");
			return -EIO;
		}
		if (cnt > nbr)		// that would be a HW bug
			cnt = nbr;
		nbr -= cnt;
		while (cnt--) {		// read as many words as there is in the FIFO
			u32 val;

			val = ioread32(jz->iobase + MSC_RXFIFO);
			*buf++ = val;
		}
	}
	return 0;

timeout:
	pr_dbg("nbr = %d\n", nbr);
	return -ETIMEDOUT;
}

static void jzmmc_clock_disable(struct jzmmc *jz)
{
	int timeout = 1000;
	u32 stat;

	iowrite32(jz->iobase + MSC_CTRL, CTRL_CLOCK_STOP);
	do
		stat = ioread32(jz->iobase + MSC_STAT);
	while ((stat & STAT_CLK_EN) && timeout--);
}

static int jzmmc_send_cmd(struct mmc_host *host, struct mmc_cmd *cmd)
{
	struct jzmmc *jz = container_of(host, struct jzmmc, host);
	struct mmc_data *data = cmd->data;
	u32 stat, mask, cmdat = 0;
	int rc;

	jzmmc_clock_disable(jz);

	mask = 0xffffffff & ~(INT_END_CMD_RES | INT_TIME_OUT_RES);

	pr_dbg("%s(%d, %08x) ...\n", __func__, MMC_CMD_IDX(cmd->cmd), cmd->arg);
	// setup command
	iowrite32(jz->iobase + MSC_CMD, MMC_CMD_IDX(cmd->cmd));
	iowrite32(jz->iobase + MSC_ARG, cmd->arg);

	if (cmd->cmd & MMC_CMD_DATA) {
		if (!data) {
			pr_err("data cmd with null buffer!\n");
			return -EINVAL;
		}

		// setup data
		mask &= ~INT_DATA_TRAN_DONE;
		cmdat |= CMDAT_DATA_EN;
		if (cmd->cmd & MMC_CMD_RDATA) {
			mask &= ~(INT_RXFIFO_RD_REQ | INT_TIME_OUT_READ);
		}

		iowrite32(jz->iobase + MSC_NOB, data->blk_nbr);
		iowrite32(jz->iobase + MSC_BLKLEN, data->blk_size);
	}

	// setup response
	pr_dbg("RTYPE = %d\n", MMC_CMD_RTYPE(cmd->cmd));
	switch (MMC_CMD_RTYPE(cmd->cmd)) {
	case 1:
		cmdat |= CMDAT_RESPONSE_R1;
		if (cmd->cmd & MMC_RSP_BUSY)
			cmdat |= CMDAT_BUSY;
		break;
	case 2:
		cmdat |= CMDAT_RESPONSE_R2;
		break;
	case 3:
		cmdat |= CMDAT_RESPONSE_R3;
		break;
	case 4:
		cmdat |= CMDAT_RESPONSE_R4;
		break;
	case 5:
		cmdat |= CMDAT_RESPONSE_R5;
		break;
	case 6:
		cmdat |= CMDAT_RESPONSE_R6;
		break;
	case 7:
		cmdat |= CMDAT_RESPONSE_R7;
		break;
	case 0:
	default:
		break;
	}

	// set init for the first command only
	if (!(jz->flags & JZ_MMC_SENT_INIT)) {
		cmdat |= CMDAT_INIT;
		jz->flags |= JZ_MMC_SENT_INIT;
	}

	cmdat |= (jz->flags & JZ_MMC_BUS_WIDTH_MASK) << 9;

	// write the data setup
	pr_dbg("cmdat = %08x\n", cmdat);
	iowrite32(jz->iobase + MSC_CMDAT, cmdat);

	// unmask interrupts
	iowrite32(jz->iobase + MSC_IMASK, mask);

	// clear interrupts
	iowrite32(jz->iobase + MSC_IFLG, 0xffffffff);

	// start the command (& the clock)
	iowrite32(jz->iobase + MSC_CTRL, CTRL_START_OP | CTRL_CLOCK_START);

	// wait for completion
	stat = jzmmc_poll_status(jz, STAT_END_CMD_RES);
	stat &= ~(STAT_CRC_READ_ERROR|STAT_CRC_WRITE_ERROR_MASK);

	iowrite32(jz->iobase + MSC_IFLG, 0xffffffff);
	rc = -ETIMEDOUT;
	if (stat & STAT_ERR) {
		pr_err("iflg = %08x\n", ioread32(jz->iobase + MSC_IFLG));
		pr_err("stat = %08x\n", stat);
		goto end;
	}

	if (MMC_CMD_RTYPE(cmd->cmd) != 0) {
		// read response: 16 bits words, most significant first

		if (cmd->cmd & MMC_RSP_136) {
			u32 *resp = &cmd->resp[0];
			u32 res;
			int i;

			// 136 bits (17 bytes), first 8 bits discarded
			res = ioread16(jz->iobase + MSC_RES) << 16;
			for (i = 0; i < 4; i++) {
				u32 tmp;

				res |= ioread16(jz->iobase + MSC_RES);
				tmp  = ioread16(jz->iobase + MSC_RES);
				resp[i] = (res << 8) | (tmp >> 8);
				res = tmp << 16;
			}
			pr_dbg("resp = %08x %08x %08x %08x\n", resp[0], resp[1], resp[2], resp[3]);
		} else {
			u32 resp;

			// 48 bits, first and second to last 8 bits discarded
			resp = ioread16(jz->iobase + MSC_RES) << 24;
			resp |= ioread16(jz->iobase + MSC_RES) << 8;
			resp |= ioread16(jz->iobase + MSC_RES) & 0xff;
			cmd->resp[0] = resp;
			pr_dbg("resp = %08x\n", resp);
		}
	}

	rc = 0;
	if (cmd->cmd & MMC_CMD_RDATA)
		rc = jzmmc_read_data(jz, cmd->data);

end:
	pr_dbg("%s() => %d\n", __func__, rc);
	return rc;
}

static int jzmmc_set_bus_width(struct mmc_host *host, uint width)
{
	struct jzmmc *jz = container_of(host, struct jzmmc, host);

	jz->flags &= ~JZ_MMC_BUS_WIDTH_MASK;
	switch (width) {
	case 8:
		jz->flags |= JZ_MMC_BUS_WIDTH_8;
		break;
	case 4:
		jz->flags |= JZ_MMC_BUS_WIDTH_4;
		break;
	case 1:
	case 0:
		jz->flags |= JZ_MMC_BUS_WIDTH_1;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int jzmmc_set_freq(struct mmc_host *host, uint freq)
{
	struct jzmmc *jz = container_of(host, struct jzmmc, host);
	uint div = 0;
	uint rate;

	rate = clk_get_rate(jz->clk);
	for (div = 0; (rate > host->freq) && (div < 7); div++) {
		rate >>= 1;
	}
	iowrite32(jz->iobase + MSC_CLKRT, div);

	return 0;
}


static void jzmmc_init_dev(struct jzmmc *jz, const char *name, ulong iobase)
{
	struct mmc_host *host = &jz->host;

	jz->flags = 0;
	jz->clk = clk_get("msc0");
	jz->iobase = ioremap(iobase, 0x100);

	host->name = name;
	host->send_cmd = jzmmc_send_cmd;
	host->set_bus_width = jzmmc_set_bus_width;
	host->set_freq = jzmmc_set_freq;
	host->reset = jzmmc_reset;
	host->get_cd = jzmmc_get_cd;

	host->vdds = MMC_VDD(34_33, 33_32);	// FIXME: board specific
	host->caps = MMC_CAP_4BIT | MMC_CAP_SDHC | MMC_CAP_HS_SD | MMC_CAP_HS_MMC;

	host->f_max = 48000000;			// FIXME: board specific
	host->f_min = host->f_max / 128;
	clk_set_rate(jz->clk, 24000000);	// FIXME: why?

	mmc_register_host(host);
}


static struct jzmmc jzmmc[1];

#include <init.h>
#include <gpio.h>
static void jzmmc_init(void)
{
	gpio_set_value(gpio_get("gpf", 19), 1);
	gpio_alt_func(gpio_get("gpe", 20), 0);
	gpio_alt_func(gpio_get("gpe", 21), 0);
	gpio_alt_func(gpio_get("gpe", 22), 0);
	gpio_alt_func(gpio_get("gpe", 23), 0);
	gpio_alt_func(gpio_get("gpe", 28), 0);
	gpio_alt_func(gpio_get("gpe", 29), 0);

	jzmmc_init_dev(&jzmmc[0], "mmc0", MSC0_BASE);
}
device_initcall(jzmmc_init);
