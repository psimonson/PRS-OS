#ifndef DISK_H
#define DISK_H

#include "defines.h"

#ifdef HARD_DISK_BOOT
typedef struct __PACKED address_packet {
	char		size;
	char		:8;
	unsigned short  blocks;
	unsigned short  buffer_offset;
	unsigned short  buffer_segment;
	unsigned long   lba;
	unsigned long   flat_buffer;
} address_packet_t;

int __REGPARM lba_read(const void *buffer, unsigned int lba,
		unsigned short blocks, unsigned char bios_drive);
#else
/* use for floppy, or as a fallback */
typedef struct {
	unsigned char spt;
	unsigned char numh;
} drive_params_t;

int __REGPARM get_drive_params(drive_params_t *p, unsigned char bios_drive);
int __REGPARM lba_read(const void *buffer, unsigned int lba,
		unsigned char blocks, unsigned char bios_drive,
		drive_params_t *p);
#endif

#endif
