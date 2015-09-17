static int sdc_send_if_cond(struct mmc_host *host, struct mmc_cmd *cmd)
{
	int rc;

	cmd->cmd = SDC_CMD_SEND_IF_COND;
	cmd->arg = 0xAA;
	if (host->vdds & MMC_VDD_MASK)
		cmd->arg |= (1 << 8);
	cmd->data = NULL;

	rc = mmc_send_cmd(host, cmd);
	if (rc)
		return rc;

	if ((cmd->resp[0] & 0xff) != 0xAA)
		return -EIO;

	return 0;
}
