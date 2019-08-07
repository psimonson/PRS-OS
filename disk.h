/*
 * Header file for my simple lba read functions.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

#ifndef DISK_H
#define DISK_H

#define USE_FLP_144	1		/* use floppy 1.44MB floppy */
#define FLP_144_SECT	(18*80*2)	/* total sectors of 1.44MB floppy */
#define FLP_144_SPT	18		/* sectors per track 1.44MB floppy */
#define FLP_144_NUMH	2		/* heads per cylinder 1.44MB floppy */

/* Drive failure defines */
#define	DISK_ERR_OK	0x00		/* success */
#define	DISK_ERR_IC	0x01		/* invalid command */
#define	DISK_ERR_CAM	0x02		/* Cannot Find Address mark */
#define	DISK_ERR_WP	0x03		/* Write protected disk */
#define	DISK_ERR_SNF	0x04		/* Sector not found */
#define	DISK_ERR_RF	0x05		/* Reset Failed */
#define	DISK_ERR_DCL	0x06		/* Disk change line 'active' */
#define	DISK_ERR_DPA	0x07		/* Drive parameter activity failed */
#define	DISK_ERR_DMA	0x08		/* DMA Overrun */
#define	DISK_ERR_DMAOB	0x09		/* Attempt to DMA over 64kb boundary */
#define	DISK_ERR_BADS	0x0A		/* Bad sector detected */
#define DISK_ERR_BADC	0x0B		/* Bad cylinder (track) detected */
#define DISK_ERR_MTYP	0x0C		/* Media type not found */
#define DISK_ERR_INS	0x0D		/* Invalid number of sectors */
#define DISK_ERR_ADDRM	0x0E		/* Control data address mark detected */
#define DISK_ERR_DMAO	0x0F		/* DMA out of range */
#define DISK_ERR_CRC	0x10		/* CRC/ECC data error */
#define DISK_ERR_ECC	0x11		/* ECC corrected data error */
#define DISK_ERR_CFAIL	0x20		/* Controller failure */
#define DISK_ERR_SFAIL	0x40		/* Seek failure */
#define DISK_ERR_TIMEO	0x80		/* Drive timed out, assumed not ready */
#define DISK_ERR_DRVNR	0xAA		/* Drive not ready */
#define DISK_ERR_UNDEF	0xBB		/* Undefined error */
#define DISK_ERR_WRITE	0xCC		/* Write fault */
#define DISK_ERR_STATUS	0xE0		/* Status error */
#define DISK_ERR_SENSE	0xFF		/* Sense operation failed */

/**
 * Drive parameters structure.
 */
typedef struct {
	unsigned char	drive;
	unsigned char	spt;
	unsigned char	numh;
	unsigned char	_unused;
	unsigned short	status;
	unsigned int	lba;
} __attribute__((packed)) drive_params_t;

/* get drive error and print message */
void __REGPARM get_drive_error(const drive_params_t *p);
/* get status of last drive operation */
int __REGPARM get_drive_status(drive_params_t *p);
/* reset disk drive */
int __REGPARM reset_drive(drive_params_t *p);
/* get drive parameters */
int __REGPARM get_drive_params(drive_params_t *p, unsigned char drive);
/* read data from disk drive */
int __REGPARM read_drive_lba(void *buf, unsigned char blocks,
	drive_params_t *p);
/* read data from disk drive */
int __REGPARM read_drive_chs(void *buf, unsigned char blocks, unsigned char c,
	unsigned char h, unsigned char s, drive_params_t* p);
/* read data from disk drive */
int __REGPARM read_drive(void *buf, unsigned char blocks, unsigned char sector,
	drive_params_t *p);
/* write data to disk drive */
int __REGPARM write_drive(const void *buf, unsigned char blocks, unsigned char sector,
	drive_params_t *p);

/* -------------------------- Helper Functions ----------------------- */

/* LBA to CHS */
void __REGPARM lba_to_chs(const drive_params_t *p, unsigned char *c, unsigned char *h,
	unsigned char *s);
/* sector to CHS */
void __REGPARM sector_to_chs(const drive_params_t *p, unsigned char sector,
	unsigned char *c, unsigned char *h, unsigned char *s);

#endif
