#include <partition.h>
#include <disk.h>
#include "parts.h"


int disk_partitions(struct disk *disk)
{
	int rc;

	rc = part_mbr(disk);
	if (rc < 0)
		return rc;

	return 0;
}
