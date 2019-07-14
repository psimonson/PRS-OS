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
	static unsigned char sector[512];
	static drive_params_t p;
	char buf[50];

	memset(bs, 0, sizeof(boot_t));
	/* read disk drive */
	if(get_drive_params(&p, 0x00))
		goto disk_error;
	if(reset_drive(&p))
		goto disk_error;
	if(read_drive_chs(sector, 1, 0, &p))
		goto disk_error;
	get_drive_error(&p);
	print("Sectors read: ");
	itoa((unsigned char)(p.status), buf);
	puts(buf);
	memcpy(bs, sector, sizeof(boot_t));
	return;

disk_error:
	get_drive_error(&p);
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

