/*
 * Simple FAT12 implementation.
 * Author: Philip R. Simonson
 * Date  : July 12, 2019
 */

#include "io.h"
#include "string.h"
#include "fat12.h"

/* Fill boot structure with boot sector data.
 */
void load_boot(boot_t *bs)
{
	memcpy(bs, (boot_t*)0x7c00, sizeof(boot_t));
}

