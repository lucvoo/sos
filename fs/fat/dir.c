#include "fat.h"
#include <string.h>


static void dump_dir_entry(const struct fat_dir_entry *dent, ulong addr)
{
	u8 status = dent->name[0];
	char name[12];

	printf("fat dent: %3d: ", addr);

	if (status == FAT_FREE) {
		printf("free\n");
		return;
	}
	if (status == FAT_DELETED) {
		printf("deleted\n");
		return;
	}

	memcpy(name, dent->name, 11);
	name[11] = '\0';
	printf("name: %s, ", name);
	printf("flags: %06b %02X, ", dent->flags, dent->cflags);
	printf("cluster: %04x%04x, ", dent->cchain_hi, dent->cchain_lo);
	printf("size: %d (0x%x)\n", dent->size, dent->size);
}

void fat_dump_dirs(const void *buf, uint n)
{
	const struct fat_dir_entry *dent = buf;
	int i = 0;

	while (n--) {
		dump_dir_entry(dent++, i++);
	}
}
