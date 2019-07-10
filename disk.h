/*
 * Header file for my simple lba read functions.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

#ifndef DISK_H
#define DISK_H

#include "defines.h"

/**
 * Drive parameters structure.
 */
typedef struct {
	unsigned char	spt;
	unsigned char	numh;
} drive_params_t;

/* get drive parameters */
int get_drive_parameters(drive_params_t *p, unsigned char drive);
/* read data from disk drive */
int lba_read(const void *buffer, unsigned int lba, unsigned short blocks,
	unsigned char drive, drive_params_t *p);

#endif
