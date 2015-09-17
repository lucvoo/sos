#include <mmc/defs.h>
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
	cmd.data = NULL;

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

	if (tries == 0)
		return -ETIMEDOUT;

	host->ocr = cmd.resp[0];
	return 0;
}

static int sdc_init(struct mmc_host *host)
{
	if (sdc_send_op_cond(host, 0))
		return -EIO;

	return 0;
}
