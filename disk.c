/*
 * Simple disk handling functions for reading disks with the
 * BIOS interrupts.
 *
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");

#include "disk.h"

boot_t const *_bs = (boot_t*)0x7c00;
FILE *_disk = (FILE*)0x7e00;

/* Reads a disk drive.
 */
unsigned short read()
{
	unsigned long t = _bs->heads * _disk->sectors;
	unsigned short c = _disk->lba / t;
	unsigned short h = (_disk->lba % t) / _disk->sectors;
	unsigned short failed = 0;
	c <<= 8;
	c |= ((_disk->lba % t) % _disk->sectors) + 1;

	/* read sectors from disk drive */
	asm("int $0x13"
		: "=a"(failed)
		: "a"(0x0200 | _size),"b"(_buffer),"c"(c),"d"((h << 8) | 0x0000));
	return (failed >> 8) | (((unsigned char)failed) != _size);
}

