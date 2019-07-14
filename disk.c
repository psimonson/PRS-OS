/*
 * Simple disk handling functions for reading disks with the
 * BIOS interrupts.
 *
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");

#include "io.h"
#include "disk.h"

/* Get the current drive error and print error message.
 */
void get_drive_error(drive_params_t *p)
{
	switch((unsigned char)p->error) {
		case DISK_ERR_OK:
			print("[DISK ERROR] : Success!\r\n");
		break;
		case DISK_ERR_IC:
			print("[DISK ERROR] : Invalid command!\r\n");
		break;
		case DISK_ERR_CAM:
			print("[DISK ERROR] : Cannot find address mark!\r\n");
		break;
		case DISK_ERR_WP:
			print("[DISK ERROR] : Write protected disk!\r\n");
		break;
		case DISK_ERR_SNF:
			print("[DISK ERROR] : Sector not found!\r\n");
		break;
		case DISK_ERR_RF:
			print("[DISK ERROR] : Reset failed!\r\n");
		break;
		case DISK_ERR_DCL:
			print("[DISK ERROR] : Disk change line 'active'!\r\n");
		break;
		case DISK_ERR_DPA:
			print("[DISK ERROR] : Drive parameter activity failed!\r\n");
		break;
		case DISK_ERR_DMA:
			print("[DISK ERROR] : DMA Overrun!\r\n");
		break;
		case DISK_ERR_DMAOB:
			print("[DISK ERROR] : Attempt to DMA over 64kb boundary!\r\n");
		break;
		case DISK_ERR_BADS:
			print("[DISK ERROR] : Bad sector detected!\r\n");
		break;
		case DISK_ERR_BADC:
			print("[DISK ERROR] : Bad cylinder/track detected!\r\n");
		break;
		case DISK_ERR_MTYP:
			print("[DISK ERROR] : Media type not found!\r\n");
		break;
		case DISK_ERR_INS:
			print("[DISK ERROR] : Invalid number of sectors!\r\n");
		break;
		case DISK_ERR_ADDRM:
			print("[DISK ERROR] : Control data address mark detected!\r\n");
		break;
		case DISK_ERR_DMAO:
			print("[DISK ERROR] : DMA out of range!\r\n");
		break;
		case DISK_ERR_CRC:
			print("[DISK ERROR] : CRC/ECC data error!\r\n");
		break;
		case DISK_ERR_ECC:
			print("[DISK ERROR] : ECC corrected data error!\r\n");
		break;
		case DISK_ERR_CFAIL:
			print("[DISK ERROR] : Controller failure!\r\n");
		break;
		case DISK_ERR_SFAIL:
			print("[DISK ERROR] : Seek failure!\r\n");
		break;
		case DISK_ERR_TIMEO:
			print("[DISK ERROR] : Drive timed out, assumed not ready!\r\n");
		break;
		case DISK_ERR_DRVNR:
			print("[DISK ERROR] : Drive not ready!\r\n");
		break;
		case DISK_ERR_UNDEF:
			print("[DISK ERROR] : Undefined error!\r\n");
		break;
		case DISK_ERR_WRITE:
			print("[DISK ERROR] : Write fault!\r\n");
		break;
		case DISK_ERR_STATUS:
			print("[DISK ERROR] : Status error!\r\n");
		break;
		case DISK_ERR_SENSE:
			print("[DISK ERROR] : Sense operation failed!\r\n");
		break;
		default:
			print("[DISK ERROR] : Unknown error!\r\n");
	}
}
/* Get the status of last drive operation.
 */
int get_drive_status(drive_params_t *p)
{
	unsigned char failed = 0;
	asm volatile(
		"int $0x13\n"
		"setcb %0\n"
		: "=r"(failed), "=a"(p->error)
		: "a"(0x0100), "d"(0x0000 | p->drive)
	);
	return failed;
}
/* Reset the disk drive.
 */
int reset_drive(drive_params_t *p)
{
	unsigned char failed = 0;
	asm volatile(
		"int $0x13\n"
		"setcb %0\n"
		: "=r"(failed), "=a"(p->error)
		: "a"(0x0000), "d"(0x0000 | p->drive)
	);
	return failed;
}
/* Gets drive parameters.
 */
int get_drive_params(drive_params_t *p, unsigned char drive)
{
	unsigned char failed = 0;
	unsigned short tmp1, tmp2;
	asm volatile(
		"int $0x13\n"
		"setcb %0\n"
		: "=r"(failed), "=a"(p->error), "=c"(tmp1), "=d"(tmp2)
		: "a"(0x0800), "d"(drive), "D"(0)
	);
	if((failed >> 8) != 0)
		return (failed >> 8);
	p->drive = drive;
	p->spt = tmp1 & 0x3f;
	p->numh = tmp2 >> 8;
	p->lba = 0;
	return failed;
}
/* Reads a disk drive.
 */
int read_drive_lba(void* buffer, unsigned long lba, unsigned char blocks,
	drive_params_t* p)
{
	unsigned char failed = 0;
	unsigned char c,h,s;
	unsigned short t;

	/* for floppy disk */
	c = lba / (p->numh * p->spt);
	t = lba % (p->numh * p->spt);
	h = t / p->spt;
	s = (t % p->spt) + 1;
	/* read sectors from disk drive */
	asm(
		"int $0x13\n"
		"setcb %0\n"
		: "=r"(failed), "=a"(p->error)
		: "a"(0x0200 | blocks), "b"((unsigned char *)buffer), "c"((c << 8) | s),
			"d"((h << 8) | p->drive)
		: "cc"
	);
	return failed;
}
/* Reads from disk drive CHS.
 */
int read_drive(void* buffer, unsigned char blocks, unsigned char c, unsigned char h,
	unsigned char s, drive_params_t* p)
{
	unsigned char failed = 0;

	/* read sectors from disk drive */
	asm(
		"int $0x13\n"
		"setcb %0\n"
		: "=r"(failed), "=a"(p->error)
		: "a"(0x0200 | blocks), "b"((unsigned char *)buffer), "c"((c << 8) | s),
			"d"((h << 8) | p->drive)
		: "cc"
	);
	return failed;
}

