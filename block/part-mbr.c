#include "parts.h"
#include <unaligned.h>
#include <kmalloc.h>
#include <errno.h>
#include <disk.h>


struct mbr_entry {
	u8	status;
	u8	chs_first[3];	// ignored
	u8	type;
	u8	chs_last[3];	// ignored
	u8	lba_off[4];
	u8	lba_nbr[4];
};


int part_mbr(struct disk *disk)
{
	const struct mbr_entry *entry;
	struct partition *part;
	u8 buff[512];		// FIXME: high stack usage!
	int rc;
	int i;
	int n;

	rc = disk->ops->read(disk, buff, 0, 1);
	if (rc)
		return 0;

	// check signature
	if ((buff[510] != 0x55) || (buff[511] != 0xAA))
		return 0;

	// count the number of valid entries
	entry = (const struct mbr_entry *) &buff[0x1BE];
	for (i = 1, n = 0; i <= 4; i++, entry++) {

		// check for valid status (0x00 or 0x80)
		if (entry->status & 0x7F)
			continue;

		// check for empty entry
		if (entry->type == 0)
			continue;

		if (readle32(entry->lba_nbr) == 0)
			continue;

		n++;
	}

	// FIXME: add support for extended partition

	disk->parts_nbr = n;
	disk->parts = kmalloc(sizeof(struct partition) * (n+1), GFP_KERN);
	if (!disk->parts)
		return -ENOMEM;

	// whole disk
	disk->parts[0].off = 0;
	disk->parts[0].nbr = disk->size;
	disk->parts[0].idx = 0;

	// parse the entries
	part = &disk->parts[1];
	entry = (const struct mbr_entry *) &buff[0x1BE];
	for (i = 1, n = 1; i <= 4; i++, entry++) {

		// check for valid status (0x00 or 0x80)
		if (entry->status & 0x7F)
			continue;

		// check for empty entry
		if (entry->type == 0)
			continue;

		part->off = readle32(entry->lba_off);
		part->nbr = readle32(entry->lba_nbr);
		part->idx = n;

		pr_info("part[%d] = (%08x, %08x)\n", part->idx, part->off, part->nbr);
		part++;
		n++;
	}

	return n;
}
