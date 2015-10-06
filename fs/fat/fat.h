#ifndef	_FAT_H_
#define	_FAT_H_


struct fat_dir_entry {
#define	FAT_FREE	0x00
#define	FAT_DELETED	0xE5
	u8	name[11];
	u8	flags;
#define	FAT_DF_RO	(1 << 0)
#define	FAT_DF_HIDDEN	(1 << 1)
#define	FAT_DF_SYSTEM	(1 << 2)
#define	FAT_DF_VLABEL	(1 << 3)
#define	FAT_DF_SUBDIR	(1 << 4)
#define	FAT_DF_ARCHIVE	(1 << 5)
#define	FAT_DF_VFAT_LFN	0x0F
	u8	cflags;
#define	FAT_LC_BASE	(1 << 3)
#define	FAT_LC_EXT	(1 << 4)
	u8	crtime_hr;
	__le16	crtime;
	__le16	crdate;
	__le16	adate;
	__le16	cchain_hi;
	__le16	mtime;
	__le16	mdate;
	__le16	cchain_lo;
	__le32	size;
};
#define	FAT_DENT_SIZE	sizeof(struct fat_dir_entry)


struct vfat_lfn_entry {
	u8	seq;
#define	VFAT_SEQ_LAST_LFN	(1 << 6)
#define	VFAT_SEQ_DELETED	FAT_DELETED
#define	VFAT_SEQ(SEQ)		((SEQ) & 0x1F)
	__le16	name1[5];
	u8	flags;
	u8	type;
	u8	csum;
	__le16	name2[6];
	__le16	cchain;
	__le32	name3[2];
};


void fat_dump_dirs(const void *dent, uint n);

#endif
