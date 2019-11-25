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

/* address to use for executable */
static unsigned char *_exec_file = (unsigned char *)0x00007e00;

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
unsigned char *load_next_sector(drive_params_t *p, boot_t *bs, char *end_list)
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
	lba = (bs->table_count*bs->sectors_per_fat)+bs->reserved_sectors+bs->hidden_sectors;
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
	if(*end_list == 0 && i++ < size)
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
	while((bytes = load_next_sector(p, bs, &end_list)) != NULL) {
		while(i <= BUFSIZ) {
			file = (entry_t*)&bytes[i];
			if(file->filename[0] == 0x00) {
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
	char found, end_list;

	/* load root directory and list files */
	found = end_list = 0;
	while((bytes = load_next_sector(p, bs, &end_list)) != NULL) {
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
/* Calculate file cluster from FAT table.
 */
void load_next_cluster(drive_params_t *p, int first, int *active)
{
	extern unsigned char *_FAT_table;
	unsigned int fat_offset;
	unsigned int ent_offset;
	unsigned short table_value;

	*active = (*active == -1 ? first : *active);
	fat_offset = *active + (*active / 2);
	ent_offset = fat_offset % 512;
	table_value = *(unsigned short*)&_FAT_table[ent_offset];

	if(*active & 0x0001)
		table_value >>= 4;
	else
		table_value &= 0x0FFF;
	*active = table_value;
	if(table_value != 0xFF8 || table_value != 0xFF7)
		read_drive((unsigned char*)_exec_file, 1, *active, p);
	else if(table_value == 0xFF8)
		*active = -1;
}
/* Load file from disk to address 0x00000200
 */
void load_file(drive_params_t *p, boot_t *bs, const char *name)
{
	char filename[12];
	entry_t *file;
	static int active = -1;
	file = find_file(p, bs, name);
	if(file == NULL) return;
	extract_filename(file, filename);
	printf("Loading %s at location: %p\r\n", filename, _exec_file);
	load_next_cluster(p, bs->reserved_sectors, &active);
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
