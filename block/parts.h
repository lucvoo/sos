#ifndef _BLOCK_PARTS_H_
#define _BLOCK_PARTS_H_


struct disk;

int disk_partitions(struct disk *disk);


/**
 * Try to parse the partition
 * returns > 0 if succesfull
 */
int part_mbr(struct disk *);

#endif
