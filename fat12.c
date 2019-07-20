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
		if((cflag = read_drive((void*)&_bs, 1, 0, p))) {
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
entry_t *load_root_next(drive_params_t *p, boot_t *bs)
{
/* TODO: Implement this function */
#if 1
	static entry_t entry;
	unsigned char retries, cflag, c, h, s;
	static char first = 1;
	int size;
	memset(&entry, 0, sizeof(entry_t));
	if(p->lba != 0 && first) return &entry;
	retries = 3;
	size = sizeof(entry_t);
	if(first)
		p->lba = bs->reserved_sectors + (bs->fats * bs->sectors_per_fat);
	else
		p->lba += sizeof(entry_t);
	lba_to_chs(p->lba, &c, &h, &s);
	printf("%d %d %d\r\n", c, h, s);
	do {
		--retries;
		if((cflag = read_drive_chs(&entry, size, c, h, s, p))) {
			if(reset_drive(p))
				goto disk_error;
			printf("Retrying... Tries left %d.\r\n", retries);
		}
	} while(retries > 0 && !cflag);
	if(cflag)
		goto disk_error;
	printf("[%x] : ", (unsigned char)(p->status >> 8));
	get_drive_error(p);
	printf("Sectors read: %d\r\n", ((unsigned char)(p->status)));
	first = 0;
	return &entry;

disk_error:
	printf("[%x] : ", (unsigned char)(p->status >> 8));
	get_drive_error(p);
	puts("Hanging system...");
	asm("cli");
	asm("hlt");
#else
	printf("Not yet implemented!\r\n");
#endif
	return 0;
}

