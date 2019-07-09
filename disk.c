/*
 * Simple disk handling functions for reading disks with the
 * BIOS interrupts.
 *
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");

#include "disk.h"

#ifdef HARD_DISK_BOOT
/* My Simple lba read function, reads a hard disk.
 */
int lba_read(const void *buffer, unsigned int lba,
		unsigned short blocks, unsigned char bios_drive)
{
	int i;
	unsigned short value = 0;
	unsigned char failed = 0;
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
			"int $0x13\n"
			: "=a"(value)
			: "a"(0x4200), "d"(bios_drive), "S"(&packet)
			: "cc"
		);
		failed = value >> 8;
		if(failed)
			break;
	}
	return failed;
}
#else
/* Get drive params, of the current drive.
 */
int get_drive_params(drive_params_t *p, unsigned char bios_drive)
{
	unsigned char failed = 0;
	unsigned short tmp1, tmp2, val;
	asm volatile(
		"int $0x13\n"
		: "=a"(val), "=c"(tmp1), "=d"(tmp2)
		: "a"(0x0800), "d"(bios_drive), "D"(0)
		: "cc", "bx"
	);
	failed = val >> 8;
	if(failed)
		return failed;
	p->spt = tmp1 & 0x3F;
	p->numh = tmp2 >> 8;
	return failed;
}
/* Reads a volume, like a floppy disk.
 */
int lba_read(const void *buffer, unsigned int lba,
		unsigned char blocks, unsigned char bios_drive,
		drive_params_t *p)
{
	unsigned char c, h, s;
	unsigned char failed;
	unsigned char num_blocks_transferred;
	unsigned short t,value;
	c = lba / (p->numh * p->spt);
	t = lba % (p->numh * p->spt);
	h = t / p->spt;
	s = (t % p->spt) + 1;
	failed = num_blocks_transferred = 0;
	asm volatile(
		"int $0x13\n"
		: "=a"(value)
		: "a"(0x0200 | blocks), "b"(buffer), "c"((c << 8) | s),
			"d"((h << 8) | bios_drive)
	);
	num_blocks_transferred = value & 0xff;
	failed = value >> 8;
	return failed || (num_blocks_transferred != blocks);
}
#endif
