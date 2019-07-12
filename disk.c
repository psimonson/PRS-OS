/*
 * Simple disk handling functions for reading disks with the
 * BIOS interrupts.
 *
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");

#include "disk.h"

/* Get the status of last drive operation.
 */
int get_drive_status(unsigned char drive)
{
	unsigned short failed = 0;
	asm("int $0x13" : "=a"(failed) : "a"(0x0100), "d"(0x0000 | drive));
	return (failed >> 4) & 0xf;
}
/* Reset the disk drive.
 */
int reset_drive(unsigned char drive)
{
	unsigned short failed = 0;
	asm("int $0x13": "=a"(failed) : "a"(0x0000), "d"(0x0000 | drive));
	return (failed >> 4) & 0xf;
}
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
	if(((failed >> 4) & 0xf) != 0)
		return ((failed >> 4) & 0xf);
	p->spt = tmp1 & 0x3f;
	p->numh = tmp2 >> 8;
	return (failed & 0xf);
}
/* Reads a disk drive.
 */
int read_drive(void* buffer, unsigned long lba, unsigned short blocks,
	unsigned char drive, drive_params_t* p)
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
	return ((((failed >> 4) & 0xf) == 0) && ((failed & 0xf) == blocks));
}

