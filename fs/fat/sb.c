#include <filesystem.h>
#include <unaligned.h>
#include <blkdev.h>
#include <errno.h>
#include <round.h>
#include <pow2.h>
#include "fat.h"


struct fat_bpb {
	uint	sector_bits;
	uint	cluster_bits;
	uint	root_dentries;
	uint	sector_nbr;
	uint	fat_off;
	uint	fat_siz;
	u8	fat_nbr;
	u8	media;
	uint	root_cluster;

	uint	dir_off;
	uint	data_off;
};


static ulong fat_calc_sector(struct fat_bpb *bpb, ulong cluster)
{
	return ((cluster-2) << bpb->cluster_bits) + bpb->data_off;
}

static int fat_read_bpb(struct blkdev *bdev, struct fat_bpb *bpb)
{
	uint reserved_sectors;
	uint nbr_clusters;
	uint dir_siz;
	u8 buf[512];			// FIXME: high stack usage!
	int bits;
	int rc;

	rc = bread(bdev, buf, 0, 1);
	if (rc)
		goto end;

	rc = -EINVAL;
	if ((bits = log_pow2(readle16(&buf[0x00B]))) < 0)
		goto end;
	bpb->sector_bits = bits;

	if ((bits = log_pow2(buf[0x00D])) < 0)
		goto end;
	bpb->cluster_bits = bits;

	reserved_sectors = readle16(&buf[0x00E]);
	if (reserved_sectors == 0)
		goto end;

	bpb->fat_nbr = buf[0x010];
	if (bpb->fat_nbr == 0)
		goto end;

	bpb->root_dentries = readle16(&buf[0x011]);

	bpb->sector_nbr = readle16(&buf[0x013]);
	if (bpb->sector_nbr == 0)
		bpb->sector_nbr = readle32(&buf[0x020]);

	bpb->media = buf[0x015];
	if (!(bpb->media == 0xF0 || bpb->media >= 0xF8))
		goto end;

	bpb->fat_siz = readle16(&buf[0x016]);
	if (bpb->fat_siz == 0) {
		bpb->fat_siz = readle32(&buf[0x024]);
		bpb->root_cluster = readle32(&buf[0x02C]);
	}


	bpb->fat_off = reserved_sectors;
	bpb->dir_off = bpb->fat_off + bpb->fat_siz * bpb->fat_nbr;

	// FIXME: check alignment of root_entries
	dir_siz = DIV_ROUND_UP(bpb->root_dentries * FAT_DENT_SIZE, 1 << bpb->sector_bits);

	bpb->data_off = bpb->dir_off + dir_siz;

	nbr_clusters = (bpb->sector_nbr - bpb->data_off) >> bpb->cluster_bits;

	rc = 0;
end:
	return rc;
}

#include <init.h>
static void __init fatfs_init(void)
{
}
driver_initcall(fatfs_init);
