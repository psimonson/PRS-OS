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
	cflag = 0;
	retries = 3;
	do {
		--retries;
		if((cflag = read_drive(&_bs, 1, 0, p))) {
			printf("Retrying... Times left %d.\r\n", retries);
			cflag = (reset_drive(p) ? 1 : 0);
		}
	} while(retries > 0 && cflag);
	if(cflag) {
		printf("Reading drive failed.\r\n");
		goto disk_error;
	}
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
/* Load FAT12 table from reserved sector.
 */
void *load_fat12(drive_params_t *p)
{
	static unsigned char FAT_table[BUFSIZ];
	char retries, cflag;
	memset(FAT_table, 0, sizeof(FAT_table));
	if(reset_drive(p))
		goto disk_error;
	cflag = 0;
	retries = 3;
	do {
		--retries;
		if((cflag = read_drive(FAT_table, 1, 1, p))) {
			printf("Retrying... Times left %d.\r\n", retries);
			cflag = (reset_drive(p) ? 1 : 0);
		}
	} while(retries > 0 && cflag);
	if(cflag) {
		printf("Reading drive failed.\r\n");
		goto disk_error;
	}
#ifdef DEBUG
	get_drive_error(p);
	printf("Sectors read: %d\r\n", ((unsigned char)(p->status)));
#endif
	return FAT_table;

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
#if 1
	static unsigned char sector[BUFSIZ];
	static unsigned char i = 0;
	unsigned short lba;
	unsigned char size;
#ifdef DEBUG
	unsigned char c, h, s;
#endif
	char retries, cflag;

	memset(sector, 0, sizeof(sector));
	retries = 3;
	cflag = 0;

	size = (32 * bs->root_entries) / bs->bytes_per_sector;
	lba = (bs->fats*bs->sectors_per_fat)+bs->reserved_sectors+bs->hidden_sectors;
	do {
		--retries;
		p->lba = lba+i;
		if((cflag = read_drive_lba(sector, 1, p)) != 0) {
			printf("Retrying... Tries left %d.\r\n", retries);
			cflag = (reset_drive(p) ? 1 : 0);
		} else {
#ifdef DEBUG
			lba_to_chs(p, &c, &h, &s);
			printf("LBA:%d = [C:%d H:%d S:%d]\r\n",
				p->lba, c, h, s);
#endif
		}
	} while(retries > 0 && cflag);
	if(cflag) {
		printf("Reading drive failed.\r\n");
		goto disk_error;
	}
#ifdef DEBUG
	lba_to_chs(p, &c, &h, &s);
	get_drive_error(p);
	printf("[DRIVE STATUS: %x]\r\n", (unsigned char)(p->status >> 8));
	printf("Sectors read: %d\r\n", ((unsigned char)(p->status)));
	printf("LBA [C:%d] [H:%d] [S:%d]\r\n", c, h, s);
#endif
	if(i++ < size)
		return sector;
	retries = 3;
	i = 0;
	return NULL;

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
	static unsigned char *bytes;
	static unsigned short i = 0;
	static entry_t *file;
	unsigned short total_size;
	char end_list;

	/* load root directory and list files */
	end_list = 0;
	total_size = 0;
	while((bytes = load_next_sector(p, bs)) != NULL) {
		if(end_list) continue;
		while(i <= BUFSIZ) {
			file = (entry_t*)&bytes[i];
			if(file->filename[0] == 0x00) {
				end_list = 1;
				break;
			} else if(file->filename[0] == 0xe5) {
				printf("File deleted.\r\n");
			} else if((file->filename[0] | 0x40) == file->filename[0]) {
				char filename[12];
				extract_filename(file, filename);
				printf("%s\r\n", filename);
				total_size += file->size;
			}
			i += sizeof(entry_t);
		}
		i = 0;
	}
	printf("Total size in directory %d bytes.\r\n", total_size);
}
/* Find file in root directory; compares it with strcmp.
 */
entry_t *find_file(drive_params_t *p, boot_t *bs, const char *filename)
{
	static unsigned short i = 0;
	static unsigned char *bytes;
	static entry_t curfile;
	entry_t *file;
	char found;

	/* load root directory and list files */
	found = 0;
	while((bytes = load_next_sector(p, bs)) != NULL) {
		if(found) continue;
		while(i <= BUFSIZ) {
			file = (entry_t*)&bytes[i];
			if(file->filename[0] == 0x00) {
				break;
			} else if(file->filename[0] == 0xe5) {
				printf("File deleted.\r\n");
			} else if((file->filename[0] | 0x40) == file->filename[0]) {
				char name[12];
				extract_filename(file, name);
				if(!strcmp(filename, name)) {
					found = 1;
					memcpy(&curfile, file, sizeof(entry_t));
				}
			}
			i += sizeof(entry_t);
		}
		i = 0;
	}
	if(found)
		return &curfile;
	return 0;
}
/* Convert file name to string.
 */
void extract_filename(const entry_t *file, char *newname)
{
	int i;
	for(i=0; i < 8 && (*newname = file->filename[i]) != ' '; newname++,i++);
	for(i=0; file->extension[i] == ' '; i++);
	if(file->extension[i] != ' ' && *newname != '.') {
		*newname++ = '.';
	}
	for(i=0; i<3 && (*newname = file->extension[i]) != ' '; newname++,i++);
	*newname = 0;
}
