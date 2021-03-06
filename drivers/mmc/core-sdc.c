#include <mmc/defs.h>
#include <byteorder.h>
#include <delay.h>


static int sdc_send_if_cond(struct mmc_host *host, struct mmc_cmd *cmd)
{
	int rc;
	u32 arg;
	u32 res;

	arg = 0xAA;
	if (host->vdds & MMC_VDD_MASK)
		arg |= (1 << 8);

	rc = mmc_simple_cmd(host, SDC_CMD_SEND_IF_COND, arg, &res);
	if (rc)
		return rc;

	if ((res & 0xff) != 0xAA)
		return -EIO;

	return 0;
}

static int sdc_send_op_cond(struct mmc_host *host, u32 ocr)
{
	struct mmc_cmd cmd;
	int tries = 256;
	int rc;

	cmd.cmd = SDC_CMD_SEND_OP_COND;
	cmd.arg = ocr;

	while (--tries) {
		rc = mmc_send_cmd(host, &cmd);
		if (rc)
			return rc;

		if (ocr == 0)		// inquiry mode
			break;

		if (cmd.resp[0] & OCR_BUSY)
			break;

		udelay(10000);		// FIXME
	}

	if (tries == 0) {
		pr_warn("card stay in busy state! (OCR = %08x)\n", cmd.resp[0]);
		return -ETIMEDOUT;
	}

	host->ocr = cmd.resp[0];
	return 0;
}

static int sdc_voltage_switch(struct mmc_host *host)
{
	pr_warn("not yet supported!\n");
	return -EINVAL;
}

static uint sdc_get_rate(struct mmc_host *host, const u32 csd[4])
{
	uint tran_speed = csd[0] & 0x3f;

	switch (tran_speed) {
	default:
	case 0x32:	return  25000000;
	case 0x5A:	return  50000000;
	case 0x0B:	return 100000000;
	case 0x2B:	return 200000000;
	}
}

static int sdc_get_scr(struct mmc_host *host)
{
	u32 scr[2];
	int rc;

	rc = mmc_read_cmd(host, SDC_CMD_SEND_SCR, 0, scr, 1, sizeof(scr));
	if (rc)
		return rc;

	host->scr = be32_to_cpu(scr[0]);
	return rc;
}

static int sdc_set_bus_width(struct mmc_host *host)
{
	int rc;

	if ((host->scr & SCR_4BIT) == 0)
		return 0;
	if ((host->caps & MMC_CAP_4BIT) == 0)
		return 0;

	rc = mmc_simple_cmd(host, SDC_CMD_SET_BUS_WIDTH, 0x2, NULL);
	if (rc)
		return rc;

	rc = mmc_set_bus_width(host, 4);
	return rc;
}

static int sdc_set_capacity(struct mmc_host *host, const u32 csd[4])
{
	uint version = csd[0] >> 30;
	u32 n;

	switch (version) {
	case 1:	n = (((csd[1] & 0x3f) << 16 | csd[2] >> 16) + 1) << 10;
		host->capacity = n;
		break;

	default:
		pr_err("CSD structure %d not (yet) supported!\n", version);
		return -EINVAL;
	}

	pr_dbg("capacity = 0x%x sectors, %d KB, %d MB\n", n, n/2, n/2048);
	return 0;
}

static int sdc_init(struct mmc_host *host)
{
	u32 csd[4];
	u32 ocr;
	u16 rca;
	int rc;

	ocr = host->vdds;
	if (host->mode == MMC_MODE_SD2)
		ocr |= OCR_CCS;
	if (host->caps & MMC_CAP_18V)
		ocr |= OCR_S18;

	rc = sdc_send_op_cond(host, ocr);
	if (rc)
		return -EIO;

	// mask out Vdds not supported by the host
	ocr = host->ocr & (host->vdds | ~MMC_VDD_MASK);

	if (host->ocr & OCR_S18)
		sdc_voltage_switch(host);

	rc = mmc_send_cid(host, ocr, NULL);
	if (rc)
		return rc;

	rc = mmc_send_rca(host, &rca);
	if (rc)
		return rc;

	rc = mmc_read_csd(host, rca, csd);
	if (rc)
		return rc;

	rc = mmc_select_card(host, rca);
	if (rc)
		return rc;

	rc = sdc_get_scr(host);
	if (rc)
		return rc;

	sdc_set_bus_width(host);

	mmc_set_freq(host, sdc_get_rate(host, csd));

	mmc_set_blocklen(host);
	sdc_set_capacity(host, csd);

	return rc;
}
