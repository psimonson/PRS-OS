/*
 * Simple FAT12 implementation.
 * Author: Philip R. Simonson
 * Date  : July 12, 2019
 */

#include "io.h"
#include "string.h"
#include "fat12.h"
#include "disk.h"

/* Fill boot structure with boot sector data.
 */
void load_boot(boot_t *bs)
{
	unsigned char *buffer;
	drive_params_t p;

	buffer = (unsigned char*)0x0500;
	/* read disk drive */
	if(get_drive_params(&p, 0x00)) {
		puts("Error: Cannot get drive params.");
		if(read_drive(buffer, 1, 0, 0, 1, &p)) {
			puts("Error: Cannot copy boot sector.");
		} else {
			memcpy(bs, buffer, sizeof(boot_t));
		}
	}
}

