/*
 * Simple disk handling functions for reading disks with the
 * BIOS interrupts.
 *
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");

#include "disk.h"

/* Gets drive parameters.
 */
int get_drive_params(drive_params_t *p, unsigned char drive)
{
	unsigned short failed = 0;
	unsigned short tmp1, tmp2;
	asm volatile("int $0x13"
		: "=a"(failed), "=c"(tmp1), "=d"(tmp2)
		: "a"(0x0800), "d"(drive), "D"(0)
		: "cc", "bx"
	);
	if((failed >> 8))
		return (failed >> 8);
	p->spt = tmp1 & 0x3F;
	p->numh = tmp2 >> 8;
	return (unsigned char)failed;
}
/* Reads a disk drive.
 */
int lba_read(const void *buffer, unsigned int lba, unsigned short blocks,
	unsigned char drive, drive_params_t *p)
{
	unsigned short failed = 0;
	unsigned char c,h,s;
	unsigned short t;

	/* for floppy disk */
	c = lba / (p->numh * p->spt);
	t = lba % (p->numh * p->spt);
	h = t / p->spt;
	s = (t % p->spt) + 1;

	/* read sectors from disk drive */
	asm("int $0x13"
		: "=a"(failed)
		: "a"(0x0200 | blocks),"b"(buffer),"c"((c << 8) | s),"d"((h << 8) | drive));
	return (failed >> 8) | (((unsigned char)failed) != blocks);
}

