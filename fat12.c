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
	static unsigned char curbs[512];
	drive_params_t p;

	/* read disk drive */
	if(get_drive_params(&p, 0x00)) {
		puts("Error: Cannot get drive params.");
		return;
	}
	if(!reset_drive(&p)) {
		if(read_drive(bs, 1, 0, 0, 1, &p)) {
			puts("Error: Cannot copy boot sector.");
			return;
		}
		memcpy(bs, curbs, sizeof(boot_t));
	}
}

