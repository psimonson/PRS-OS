/*
 * Header file for a simple FAT12 file system.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

#ifndef FAT12_H
#define FAT12_H

/**
 * Boot structure.
 */
typedef struct {
	char		_a[3];
	char		name[8];
	unsigned short	bytes_per_sector;
	unsigned char	sectors_per_cluster;
	unsigned short	reserved_sectors;
	unsigned char	fats;
	unsigned short	root_entries;
	unsigned short	total_sectors;
	unsigned short	sectors_per_fat;
	unsigned short	sectors_per_track;
	unsigned short	heads_per_cylinder;
	unsigned long	hidden_sectors;
	unsigned long	total_sectors_big;
	unsigned short	drive_index;
	unsigned char	_unused;
	unsigned char	ext_boot_sig;
	unsigned long	id;
	char		label[11];
	char		type[8];
	unsigned char	_c[448];
	unsigned short	sig;
} __attribute__((packed)) boot_t;

/**
 * Structure for FAT file entry.
 */
typedef struct {
	char		filename[8];
	char		extension[3];
	unsigned char	attributes;
	unsigned short	reserved;
	unsigned short	create_time;
	unsigned short	create_date;
	unsigned short	last_access_date;
	unsigned short	_ignore_fat12;
	unsigned short	last_write_time;
	unsigned short	last_write_date;
	unsigned short	cluster;
	unsigned long	size;
} __attribute__((packed)) entry_t;

/**
 * Structure for disk drive.
 */
typedef struct {
	unsigned char sectors;
	unsigned long lba;
} FILE;

/* try and fill boot structure */
void load_boot(boot_t *bs);

#endif

