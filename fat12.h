/*
 * Header file for a simple FAT12 file system.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

#ifndef FAT12_H
#define FAT12_H

#include "defines.h"
#include "disk.h"

/* FAT32 Extended boot structure.
 */
typedef struct fat32 {
	unsigned int	table_size_32;
	unsigned short	extended_flags;
	unsigned short	fat_version;
	unsigned int	root_cluster;
	unsigned short	fat_info;
	unsigned short	backup_boot_sector;
	unsigned char	reserved_0[12];
	unsigned char	drive_number;
	unsigned char	reserved_1;
	unsigned char	boot_sig;
	unsigned int	volume_id;
	unsigned char	volule_label[11];
	unsigned char	fat_type[8];
} __PACKED fat32_t;
/* FAT12 Extended boot structure.
 */
typedef struct fat12 {
	unsigned char	bios_drive;
	unsigned char	reserved1;
	unsigned char	boot_sig;
	unsigned int	volume_id;
	unsigned char	volume_label[11];
	unsigned char	fat_type[8];
} __PACKED fat12_t;
/* Boot sector structure.
 */
typedef struct boot {
	unsigned char	bootjmp[3];
	unsigned char	oem_name[8];
	unsigned short	bytes_per_sector;
	unsigned char	sectors_per_cluster;
	unsigned short	reserved_sectors;
	unsigned char	table_count;
	unsigned short	root_entries;
	unsigned short	total_sectors;
	unsigned char	media_type;
	unsigned short	sectors_per_fat;
	unsigned short	sectors_per_track;
	unsigned short	heads_per_cylinder;
	unsigned int	hidden_sectors;
	unsigned int	total_sectors_big;
	unsigned char	extended_section[54];
} __PACKED boot_t;
/* Structure for LFN entry.
 */
typedef struct lfn_entry {
	unsigned char	type;
	unsigned char	sname[10];
	unsigned char	attr;
	unsigned char	long_type;
	unsigned char	checksum;
	unsigned char	lname[12];
	unsigned short	reserved;
	unsigned char	fname[4];
} __PACKED lfn_entry_t;
/* Structure for FAT file entry.
 */
typedef struct entry {
	unsigned char	_lfn[32];
	unsigned char	filename[8];
	unsigned char	extension[3];
	unsigned char	attributes;
	unsigned char	reserved;
	unsigned char	create_time_ms;
	unsigned short	create_time;
	unsigned short	create_date;
	unsigned short	last_access_date;
	unsigned short	ignore_in_fat12;
	unsigned short	last_write_time;
	unsigned short	last_write_date;
	unsigned short	cluster;
	unsigned int	size;
} __PACKED entry_t;

/* try and fill boot structure */
boot_t *load_boot(drive_params_t *p);
/* try and load FAT12 table */
void *load_fat12(drive_params_t *p);
/* try and get root directory entries */
unsigned char *load_next_sector(drive_params_t *p, boot_t *bs, char *end_list);
/* list directory structure */
void list_directory(drive_params_t *p, boot_t *bs);
/* find file in root directory */
entry_t *find_file(drive_params_t *p, boot_t *bs, const char *filename);
/* convert filename into cstring */
void extract_filename(const entry_t *filename, char *newname);

#endif

