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
void __REGPARM get_drive_error(const drive_params_t *p)
{
	switch((unsigned char)(p->status >> 8)) {
		case DISK_ERR_OK:
			print("[DISK STATUS] : Success!\r\n");
		break;
		case DISK_ERR_IC:
			print("[DISK STATUS] : Invalid command!\r\n");
		break;
		case DISK_ERR_CAM:
			print("[DISK STATUS] : Cannot find address mark!\r\n");
		break;
		case DISK_ERR_WP:
			print("[DISK STATUS] : Write protected disk!\r\n");
		break;
		case DISK_ERR_SNF:
			print("[DISK STATUS] : Sector not found!\r\n");
		break;
		case DISK_ERR_RF:
			print("[DISK STATUS] : Reset failed!\r\n");
		break;
		case DISK_ERR_DCL:
			print("[DISK STATUS] : Disk change line 'active'!\r\n");
		break;
		case DISK_ERR_DPA:
			print("[DISK STATUS] : Drive parameter activity failed!\r\n");
		break;
		case DISK_ERR_DMA:
			print("[DISK STATUS] : DMA Overrun!\r\n");
		break;
		case DISK_ERR_DMAOB:
			print("[DISK STATUS] : Attempt to DMA over 64kb boundary!\r\n");
		break;
		case DISK_ERR_BADS:
			print("[DISK STATUS] : Bad sector detected!\r\n");
		break;
		case DISK_ERR_BADC:
			print("[DISK STATUS] : Bad cylinder/track detected!\r\n");
		break;
		case DISK_ERR_MTYP:
			print("[DISK STATUS] : Media type not found!\r\n");
		break;
		case DISK_ERR_INS:
			print("[DISK STATUS] : Invalid number of sectors!\r\n");
		break;
		case DISK_ERR_ADDRM:
			print("[DISK STATUS] : Control data address mark detected!\r\n");
		break;
		case DISK_ERR_DMAO:
			print("[DISK STATUS] : DMA out of range!\r\n");
		break;
		case DISK_ERR_CRC:
			print("[DISK STATUS] : CRC/ECC data error!\r\n");
		break;
		case DISK_ERR_ECC:
			print("[DISK STATUS] : ECC corrected data error!\r\n");
		break;
		case DISK_ERR_CFAIL:
			print("[DISK STATUS] : Controller failure!\r\n");
		break;
		case DISK_ERR_SFAIL:
			print("[DISK STATUS] : Seek failure!\r\n");
		break;
		case DISK_ERR_TIMEO:
			print("[DISK STATUS] : Drive timed out, assumed not ready!\r\n");
		break;
		case DISK_ERR_DRVNR:
			print("[DISK STATUS] : Drive not ready!\r\n");
		break;
		case DISK_ERR_UNDEF:
			print("[DISK STATUS] : Undefined error!\r\n");
		break;
		case DISK_ERR_WRITE:
			print("[DISK STATUS] : Write fault!\r\n");
		break;
		case DISK_ERR_STATUS:
			print("[DISK STATUS] : Status error!\r\n");
		break;
		case DISK_ERR_SENSE:
			print("[DISK STATUS] : Sense operation failed!\r\n");
		break;
		default:
			print("[DISK STATUS] : Unknown error!\r\n");
	}
}
/* Get the status of last drive operation.
 */
int __REGPARM get_drive_status(drive_params_t *p)
{
	unsigned char failed = 0;

	asm volatile(
		"push %%ds\n\t"
		"push %%es\n\t"
		"int $0x13\n\t"
		"setcb %0\n\t"
		"pop %%es\n\t"
		"pop %%ds\n\t"
		: "=r"(failed), "=a"(p->status)
		: "a"(0x0100), "d"(0x0000 | p->drive)
	);
	return failed;
}
/* Reset the disk drive.
 */
int __REGPARM reset_drive(drive_params_t *p)
{
	unsigned char failed = 0;

	p->status = 0;
	asm volatile(
		"push %%ds\n\t"
		"push %%es\n\t"
		"int $0x13\n\t"
		"setcb %0\n\t"
		"pop %%es\n\t"
		"pop %%ds\n\t"
		: "=r"(failed), "=a"(p->status)
		: "a"(0x0000), "d"(0x0000 | p->drive)
	);
	return failed;
}
/* Gets drive parameters.
 */
int __REGPARM get_drive_params(drive_params_t *p, unsigned char drive)
{
	unsigned char failed = 0;
	unsigned short tmp1, tmp2;

	asm volatile(
		"push %%ds\n\t"
		"push %%es\n\t"
		"int $0x13\n\t"
		"setcb %0\n\t"
		"pop %%es\n\t"
		"pop %%ds\n\t"
		: "=r"(failed), "=a"(p->status), "=c"(tmp1), "=d"(tmp2)
		: "a"(0x0800), "d"(drive), "D"(0)
	);
	if(!failed) {
		p->drive = drive;
		p->spt = tmp1 & 0x3f;
		p->numh = tmp2 >> 8;
		p->lba = 0;
	}
	return failed;
}
/* Reads a disk drive using LBA.
 */
int __REGPARM read_drive_lba(void *buf, unsigned char blocks, drive_params_t* p)
{
	unsigned char failed = 0;
	unsigned char c,h,s;

	lba_to_chs(p, &c, &h, &s);

	/* read sectors from disk drive */
	asm volatile(
		"push %%ds\n\t"
		"push %%es\n\t"
		"int $0x13\n\t"
		"setcb %0\n\t"
		"pop %%es\n\t"
		"pop %%ds\n\t"
		: "=r"(failed), "=a"(p->status)
		: "a"(0x0200 | blocks), "b"(buf),
			"c"((c << 8) | s), "d"((h << 8) | p->drive)
	);
	return failed;
}
/* Reads a disk drive using CHS.
 */
int __REGPARM read_drive_chs(void *buf, unsigned char blocks, unsigned char c,
	unsigned char h, unsigned char s, drive_params_t* p)
{
	unsigned char failed = 0;

	/* read sectors from disk drive */
	asm volatile(
		"push %%ds\n\t"
		"push %%es\n\t"
		"int $0x13\n\t"
		"setcb %0\n\t"
		"pop %%es\n\t"
		"pop %%ds\n\t"
		: "=r"(failed), "=a"(p->status)
		: "a"(0x0200 | blocks), "b"(buf),
			"c"((c << 8) | s), "d"((h << 8) | p->drive)
	);
	return failed;
}
/* Reads a disk drive by sectors. (floppy only)
 */
int __REGPARM read_drive(void *buf, unsigned char blocks, unsigned char sector,
	drive_params_t* p)
{
	unsigned char failed = 0;
	unsigned char c, h, s;

	sector_to_chs(p, sector, &c, &h, &s);

	/* read sectors from disk drive */
	asm volatile(
		"push %%ds\n\t"
		"push %%es\n\t"
		"int $0x13\n\t"
		"setcb %0\n\t"
		"pop %%es\n\t"
		"pop %%ds\n\t"
		: "=r"(failed), "=a"(p->status)
		: "a"(0x0200 | blocks), "b"(buf),
			"c"((c << 8) | s), "d"((h << 8) | p->drive)
	);
	return failed;
}
/* Write to a disk drive. (floppy only)
 */
int __REGPARM write_drive(const void *buf, unsigned char blocks, unsigned char sector,
	drive_params_t *p)
{
	unsigned char failed = 0;
	unsigned char c, h, s;

	sector_to_chs(p, sector, &c, &h, &s);

	/* write blocks starting at sector */
	asm volatile(
		"push %%ds\n\t"
		"push %%es\n\t"
		"int $0x13\n\t"
		"setcb %0\n\t"
		"pop %%es\n\t"
		"pop %%ds\n\t"
		: "=r"(failed), "=a"(p->status)
		: "a"(0x0300 | blocks), "b"(buf), "c"((c << 8) | s),
			"d"((h << 8) | p->drive)
	);
	return failed;
}

/* -------------------------- Helper Functions ----------------------- */

/* LBA to CHS conversion function.
 */
void __REGPARM lba_to_chs(const drive_params_t *p, unsigned char *c,
	unsigned char *h, unsigned char *s)
{
	unsigned int temp;
#if USE_FLP_144
	temp = p->lba / FLP_144_SPT;
	*c = temp / FLP_144_NUMH;
	*h = temp % FLP_144_NUMH;
	*s = (p->lba % FLP_144_SPT) + 1;
#else
	temp = p->lba / p->spt;
	*c = temp / p->numh;
	*h = temp % p->numh;
	*s = (p->lba % p->spt) + 1;
#endif
}
/* CHS to sector conversion.
 */
void __REGPARM sector_to_chs(const drive_params_t *p, unsigned char sector,
	unsigned char *c, unsigned char *h, unsigned char *s)
{
#if USE_FLP_144
	*c = (sector / FLP_144_SPT) / FLP_144_NUMH;
	*h = (sector / FLP_144_SPT) % FLP_144_NUMH;
	*s = (sector % FLP_144_SPT) + 1;
	(void)p;
#else
	*c = (sector / p->spt) / p->numh;
	*h = (sector / p->spt) % p->numh;
	*s = (sector % p->spt) + 1;
#endif
}

