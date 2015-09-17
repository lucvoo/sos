#include <mmc.h>
#include <mmc/cmds.h>
#include <mmc/drivers.h>
#include <errno.h>
#include <string.h>


static int mmc_set_bus_width(struct mmc_host *host, uint width)
{
	int rc;

	rc = host->set_bus_width(host, width);
	if (!rc)
		host->bus_width = width;

	return rc;
}

static int mmc_set_freq(struct mmc_host *host, uint freq)
{
	int rc;

	rc = host->set_freq(host, freq);
	if (!rc)
		host->freq = freq;

	return rc;
}


static int mmc_send_cmd(struct mmc_host *host, struct mmc_cmd *cmd)
{
	if (cmd->cmd & MMC_CMD_ACMD) {
		struct mmc_cmd acmd;
		int rc;

		acmd.cmd = MMC_CMD_APP_CMD;
		acmd.arg = host->rca;
		acmd.data = NULL;

		rc = host->send_cmd(host, &acmd);
		if (rc)
			return rc;

		// FIXME: check response
	}

	return host->send_cmd(host, cmd);
}

static int mmc_simple_cmd(struct mmc_host *host, uint cmd, uint arg, u32 *res)
{
	struct mmc_cmd c;
	int rc;

	c.cmd = cmd;
	c.arg = arg;
	c.data = NULL;

	rc = mmc_send_cmd(host, &c);
	if (rc)
		return rc;

	if (res) {
		if (cmd & MMC_RSP_136)
			memcpy(res, c.resp, sizeof(c.resp));
		else
			*res = c.resp[0];
	}

	return rc;
}

static int mmc_go_idle(struct mmc_host *host)
{
	int rc;

	rc = mmc_simple_cmd(host, MMC_CMD_GO_IDLE, 0, NULL);
	return rc;
}


static int mmc_send_cid(struct mmc_host *host, u32 ocr, u32 cid[4])
{
	int rc;

	rc = mmc_simple_cmd(host, MMC_CMD_ALL_SEND_CID, ocr, cid);
	if (rc)
		return rc;

	if (cid)
		pr_note("CID: product name: %c%c%c%c%c\n",cid[0]>>0,cid[1]>>24,cid[1]>>16,cid[1]>>8,cid[1]);

	return rc;
}

static int mmc_send_rca(struct mmc_host *host, u16 *rca)
{
	u32 res;
	int rc;

	rc = mmc_simple_cmd(host, MMC_CMD_SEND_RCA, 0, &res);
	if (rc)
		return rc;

	host->rca = res & 0xffff0000;

	if (rca)
		*rca = res >> 16;

	return rc;
}

static int mmc_read_csd(struct mmc_host *host, uint rca, u32 *csd)
{
	int rc;

	rc = mmc_simple_cmd(host, MMC_CMD_SEND_CSD, rca << 16, csd);
	return rc;
}

static int mmc_select_card(struct mmc_host *host, uint rca)
{
	int rc;

	rc = mmc_simple_cmd(host, MMC_CMD_SELECT_CARD, rca << 16, NULL);
	return rc;
}


#include "core-sdc.c"


static int mmc_detect_mode(struct mmc_host *host)
{
	struct mmc_cmd cmd;
	int rc;

	rc = sdc_send_if_cond(host, &cmd);
	if (rc == 0) {
		host->mode = MMC_MODE_SD2;	// FIXME: card?

		if ((cmd.resp[0] & 0xF00) != 0x100)
			return -EIO;

		return 0;
	}

	// FIXME
	pr_err("card type not yet supported (SD V1.1 or MMC)\n");
	return -EIO;
}

static int mmc_init_host(struct mmc_host *host)
{
	int rc;

	if (host->get_cd(host) == 0) {
		return -ENOMEDIUM;
	}

	rc = host->reset(host);
	if (rc)
		return rc;

	mmc_set_bus_width(host, 1);
	mmc_set_freq(host, 400000);

	// Reset the card
	rc = mmc_go_idle(host);
	if (rc)
		return rc;

	mmc_detect_mode(host);
	if (host->mode & MMC_MODE_SD)
		rc = sdc_init(host);
	else
		return -EIO;

	return rc;
}


int mmc_register_host(struct mmc_host *host)
{
	int rc;

	rc = mmc_init_host(host);

	return rc;
}
