/*
 * Simple FAT12 boot sector loading.
 * Author: Philip R. Simonson
 * Date  : July 12, 2019
 */

asm(".code16gcc");

#include "io.h"
#include "string.h"
#include "fat12.h"
#include "disk.h"

#define MAXROOT 255

/* Fill boot structure with boot sector data.
 */
boot_t *load_boot(drive_params_t *p)
{
	static boot_t _bs;
	char retries, cflag;
	memset(&_bs, 0, sizeof(boot_t));
	if(reset_drive(p))
		goto disk_error;
	retries = 3;
	do {
		--retries;
		if((cflag = read_drive_chs((void*)&_bs, 1, 0, p))) {
			if(reset_drive(p))
				goto disk_error;
			printf("Retrying... Times left %d.\r\n", retries);
		}

	} while(retries > 0 && !cflag);
	if(cflag)
		goto disk_error;
	get_drive_error(p);
	printf("Sectors read: %d\r\n", ((unsigned char)(p->status)));
	return &_bs;

disk_error:
	get_drive_error(p);
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
	return 0;
}
/* Get file entry from FAT12 filesystem.
 */
entry_t *load_root(drive_params_t *p, boot_t *bs)
{
/* TODO: Implement this function */
#if 0
	static entry_t _entry[MAXROOT];
	char retries, cflag;
	int size;
	memset(&_entry, 0, sizeof(entry_t));
	if(reset_drive(p))
		goto disk_error;
	retries = 3;
	p->lba = bs->reserved_sectors + (bs->fats * bs->sectors_per_fat);
	size = bs->root_entries * sizeof(entry_t) / bs->bytes_per_sector;
	do {
		--retries;
		if((cflag = read_drive_lba(&_entry, size, p->lba, p))) {
			if(reset_drive(p))
				goto disk_error;
			printf("Retrying... Tries left %d.\r\n", retries);
		}
	} while(retries > 0 && !cflag);
	if(cflag)
		goto disk_error;
	get_drive_error(p);
	printf("Sectors read: %d\r\n", ((unsigned char)(p->status)));
	p->lba = 0;
	return &_entry[0];

disk_error:
	get_drive_error(p);
	puts("Hanging system...");
	asm("cli");
	asm("hlt");
#else
	printf("Not yet implemented!\r\n");
#endif
	return 0;
}

