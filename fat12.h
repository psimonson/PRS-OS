/*
 * Header file for a simple FAT12 file system.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

#ifndef FAT12_H
#define FAT12_H

#include "disk.h"

/* Boot sector structure.
 */
typedef struct boot {
	unsigned char	_a[3];
	unsigned char	name[8];
	unsigned short	bytes_per_sector;
	unsigned char	sectors_per_cluster;
	unsigned short	reserved_sectors;
	unsigned char	fats;
	unsigned short	root_entries;
	unsigned short	total_sectors;
	unsigned char	media;
	unsigned short	sectors_per_fat;
	unsigned short	sectors_per_track;
	unsigned short	heads_per_cylinder;
	unsigned int	hidden_sectors;
	unsigned int	total_sectors_big;
	unsigned short	drive_index;
	unsigned char	_unused;
	unsigned char	ext_boot_sig;
	unsigned int	id;
	unsigned char	label[11];
	unsigned char	type[8];
	unsigned char	_c[448];
	unsigned short	sig;
} __attribute__((packed)) boot_t;

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
} __attribute__((packed)) lfn_entry_t;

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
} __attribute__((packed)) entry_t;

/* try and fill boot structure */
boot_t *load_boot(drive_params_t *p);
/* try and load FAT12 table */
void *load_fat12(drive_params_t *p);
/* try and get root directory entries */
unsigned char *load_next_sector(drive_params_t *p, boot_t *bs);
/* list directory structure */
void list_directory(drive_params_t *p, boot_t *bs);
/* find file in root directory */
entry_t *find_file(drive_params_t *p, boot_t *bs, const char *filename);
/* convert filename into cstring */
void extract_filename(const entry_t *filename, char *newname);

#endif

