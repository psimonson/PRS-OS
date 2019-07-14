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

	/* read disk drive */
	if(get_drive_params(&p, 0x00))
		goto disk_error;

	if(!reset_drive(&p)) {
		if(read_drive_floppy(bs, 1, &p))
			goto disk_error;
	}
	return;

disk_error:
	get_drive_error(&p);
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

