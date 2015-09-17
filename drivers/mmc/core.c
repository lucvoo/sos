#include <mmc.h>
#include <mmc/drivers.h>
#include <errno.h>


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
	return host->send_cmd(host, cmd);
}

static int mmc_go_idle(struct mmc_host *host)
{
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

	return rc;
}


int mmc_register_host(struct mmc_host *host)
{
	int rc;

	rc = mmc_init_host(host);

	return rc;
}
