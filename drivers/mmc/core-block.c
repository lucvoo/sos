#include <disk.h>
#include <errno.h>


static int mmc_bcheck(struct disk *disk, sector_t sec, uint nbr)
{
	sector_t max = disk->size;

	if (sec >= max)
		return -EINVAL;
	if (nbr >= max)
		return -EINVAL;
	if (nbr >= (max - sec))
		return -EINVAL;

	return 0;
}

static int mmc_bread(struct disk *disk, void *buff, sector_t sec, uint nbr)
{
	struct mmc_host *host = disk->priv;
	int rc;

	rc = mmc_bcheck(disk, sec, nbr);
	if (rc)
		return rc;

	rc = mmc_read_sectors(host, buff, sec, nbr);
	return rc;
}

static int mmc_bwrite(struct disk *disk, sector_t sec, uint nbr, const void *buff)
{
	struct mmc_host *host = disk->priv;
	int rc;

	rc = mmc_bcheck(disk, sec, nbr);
	if (rc)
		return rc;

	rc = mmc_write_sectors(host, sec, nbr, buff);
	return rc;
}

static struct disk_ops mmc_blkops = {
	.read = mmc_bread,
	.write = mmc_bwrite,
};
