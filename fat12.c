/*
 * Simple FAT12 boot sector loading.
 * Author: Philip R. Simonson
 * Date  : July 12, 2019
 */

asm(".code16gcc");

#include "io.h"
#include "string.h"
#include "fat12.h"
#include "disk.h"

/* Fill boot structure with boot sector data.
 */
boot_t *load_boot(drive_params_t *p)
{
	static boot_t _bs;
	char retries, cflag;
	memset(&_bs, 0, sizeof(boot_t));
	if(reset_drive(p))
		goto disk_error;
	retries = 3;
	do {
		--retries;
		if((cflag = read_drive((void*)&_bs, 1, 0, p))) {
			if(!reset_drive(p))
				goto disk_error;
			printf("Retrying... Times left %d.\r\n", retries);
		}

	} while(retries > 0 && !cflag);
	if(cflag)
		goto disk_error;
#ifdef DEBUG
	get_drive_error(p);
	printf("Sectors read: %d\r\n", ((unsigned char)(p->status)));
#endif
	return &_bs;

disk_error:
	get_drive_error(p);
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
	return 0;
}
/* Get file root directory from FAT12 filesystem.
 */
unsigned char *load_next_sector(drive_params_t *p, boot_t *bs)
{
/* TODO: Implement this function */
#if 1
	static unsigned char sector[BUFSIZ];
	unsigned char retries, cflag, c, h, s;
	static char first = 1;
	static unsigned char i = 0;

	memset(sector, 0, sizeof(sector));
	if(first)
		p->lba = bs->reserved_sectors + (bs->fats * bs->sectors_per_fat);
	else
		p->lba += i;
	retries = 3;
	lba_to_chs(p->lba, &c, &h, &s);
	do {
		--retries;
		if((cflag = read_drive_chs(sector, 1, c, h, s, p))) {
			if(reset_drive(p))
				goto disk_error;
			printf("Retrying... Tries left %d.\r\n", retries);
		}
	} while(retries > 0 && cflag);
	if(cflag)
		goto disk_error;
#ifdef DEBUG
	get_drive_error(p);
	printf("[DRIVE STATUS: %x]\r\n", (unsigned char)(p->status >> 8));
	printf("Sectors read: %d\r\n", ((unsigned char)(p->status)));
	printf("LBA [C:%d] [H:%d] [S:%d]\r\n", c, h, s);
#endif
	first = 0;
	if(i >= bs->root_entries) {
		memset(sector, 0, sizeof(sector));
		retries = 3;
		first = 1;
		i = 0;
		return NULL;
	}
	++i;
	return sector;

disk_error:
	get_drive_error(p);
#ifdef DEBUG
	printf("[%x] : ", (unsigned char)(p->status >> 8));
#endif
	puts("Hanging system...");
	asm("cli");
	asm("hlt");
#else
	printf("Not yet implemented!\r\n");
#endif
	return 0;
}
/* List root directory structure; displays name and size of files.
 */
void list_directory(drive_params_t *p, boot_t *bs)
{
	unsigned char *bytes;
	unsigned short i;
	entry_t *file;

	printf("Listing root directory...\r\n");

	/* load root directory and list files */
	i = sizeof(entry_t);
	while((bytes = load_next_sector(p, bs)) != NULL) {
		while(i < bs->root_entries) {
			file = (entry_t*)&bytes[i];
			if(file->filename[0] == 0xe5) {
				printf("File deleted.\r\n");
			} else if((file->filename[0] | 0x40) == file->filename[0]) {
				char filename[12];
				conv_filename(file->filename, filename);
				printf("File Name: %s\r\n", filename);
				printf("File Size: %d\r\n", file->size);
			}
			i += sizeof(entry_t)*2;
		}
	}
	printf("End of directory listing.\r\n");
}
/* Find file in root directory; compares it with strcmp.
 */
void find_file(drive_params_t *p, boot_t *bs, const char *filename)
{
	unsigned char *bytes;
	unsigned short i;
	entry_t *file;
	char found = 0;

	printf("Finding file in root directory...\r\n");

	/* load root directory and list files */
	i = sizeof(entry_t);
	while((bytes = load_next_sector(p, bs)) != NULL) {
		while(i < bs->root_entries) {
			file = (entry_t*)&bytes[i];
			if(file->filename[0] == 0xe5) {
				printf("File deleted.\r\n");
			} else if((file->filename[0] | 0x40) == file->filename[0]) {
				if(!memcmp(filename, file->filename, 11))
						found = 1;
			}
			i += sizeof(entry_t)*2;
		}
	}
	if(found)
		printf("File found.\r\n");
	else
		printf("File not found.\r\n");
}
/* Convert file name to string.
 */
void conv_filename(unsigned char *filename, char *newname)
{
	int i;
	for(i=0; (*newname = *filename) != ' '; newname++,filename++,i++);
	while(i<12 && *filename == ' ') filename++;
	if(*(filename-1) == ' ') {
		*newname++ = '.';
		i++;
	}
	while(i<12 && (*newname++ = *filename++));
	*newname = 0;
}
