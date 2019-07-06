/*
 * Simple disk handling functions for reading disks with the
 * BIOS interrupts.
 *
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc\n");

#include "disk.h"

#ifdef HARD_DISK_BOOT
/* My Simple lba read function, reads a hard disk.
 */
int __REGPARM lba_read(const void *buffer, unsigned int lba,
		unsigned short blocks, unsigned char bios_drive)
{
	int i;
	unsigned short failed = 0;
	address_packet_t packet;
	packet.size = sizeof(address_packet_t);
	packet.blocks = blocks;
	packet.buffer_offset = 0xFFFF;
	packet.buffer_segment = 0xFFFF;
	packet.lba = lba;
	packet.flat_buffer = (unsigned long)buffer;
	for(i = 0; i < 3; i++) {
		packet.blocks = blocks;
		asm volatile(
			"movw $0, %0\n"
			"int $0x13\n"
			"setcb %0\n"
			: "=m"(failed)
			: "a"(0x4200), "d"(bios_drive), "S"(&packet)
			: "cc"
		);
		if(!failed)
			break;
	}
	return failed;
}
#else
/* Get drive params, of the current drive.
 */
int __REGPARM get_drive_params(drive_params_t *p, unsigned char bios_drive)
{
	unsigned short failed = 0;
	unsigned short tmp1, tmp2;
	asm volatile(
		"movw $0, %0\n"
		"int $0x13\n"
		"setcb %0\n"
		: "=m"(failed), "=c"(tmp1), "=d"(tmp2)
		: "a"(0x0800), "d"(bios_drive), "D"(0)
		: "cc", "bx"
	);
	if(failed)
		return failed;
	p->spt = tmp1 & 0x3F;
	p->numh = tmp2 >> 8;
	return failed;
}
/* Reads a volume, like a floppy disk.
 */
int __REGPARM lba_read(const void *buffer, unsigned int lba,
		unsigned char blocks, unsigned char bios_drive,
		drive_params_t *p)
{
	unsigned char c, h, s;
	unsigned char failed;
	unsigned char num_blocks_transferred;
	unsigned short t;
	c = lba / (p->numh * p->spt);
	t = lba % (p->numh * p->spt);
	h = t / p->spt;
	s = (t % p->spt) + 1;
	failed = num_blocks_transferred = 0;
	asm volatile(
		"movw $0, %0\n"
		"int $0x13\n"
		"setcb %0\n"
		: "=m"(failed), "=a"(num_blocks_transferred)
		: "a"(0x0200 | blocks), "b"(buffer), "c"((c << 8) | s),
			"d"((h << 8) | bios_drive)
	);
	return failed || (num_blocks_transferred != blocks);
}
#endif
