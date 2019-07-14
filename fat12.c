/*
 * Simple FAT12 implementation.
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
void load_boot(boot_t *bs)
{
	drive_params_t p;
	char buf[50];

	/* read disk drive */
	if(get_drive_params(&p, 0x00))
		goto disk_error;
	if(reset_drive(&p))
		goto disk_error;
	if(read_drive_lba(bs, 0, 1, &p))
		goto disk_error;
	get_drive_error(&p);
	print("Sectors read: ");
	itoa((unsigned char)p.status, buf);
	puts(buf);
	return;

disk_error:
	get_drive_error(&p);
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

