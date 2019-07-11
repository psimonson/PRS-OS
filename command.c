/*
 * Main program for my boot code, just displays some graphics.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");
asm("jmp main");

#include "io.h"
#include "string.h"
#include "time.h"
#include "disk.h"
#include "fat12.h"

#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x00"

/* Entry point for my command shell.
 */
void main()
{
	extern int shell();
	unsigned short* buffer;
	entry_t const* entry;
	boot_t const* bs = (boot_t const*)1000;
	unsigned long lba;
	unsigned short blocks;
	drive_params_t p;
	char buf[50];

	/* setup data segment */
	asm("push %cs");
	asm("pop %ds");

	/* reset disk drive */
	if(reset_drive(0x00)) {
		print("[ERROR] : Could not reset disk drive.\r\n");
		goto error;
	}
	/* get current drive */
	if(get_drive_params(&p, 0x00)) {
		print("[ERROR] : Failed to get current drive info.\r\n");
		goto error;
	}
	/* reset disk drive */
	if(reset_drive(0x00)) {
		print("[ERROR] : Could not reset disk drive.\r\n");
		goto error;
	}
	/* read disk into memory */
	if(!read_drive((void*)bs, 0, 1, 0x00, &p)) {
		print("[ERROR] : Could not load disk sectors.\r\n");
		goto error;
	}
	/* reset disk drive */
	if(reset_drive(0x00)) {
		print("[ERROR] : Could not reset disk drive.\r\n");
		goto error;
	}
	buffer = (unsigned short*)0x0500;
	lba = bs->reserved_sectors + (bs->fats * bs->sectors_per_fat);
	blocks = bs->root_entries * sizeof(entry_t) / bs->bytes_per_sector;
	itoa(lba, buf);
	puts(buf);
	itoa(blocks, buf);
	puts(buf);
	if(!read_drive(buffer, lba, blocks, 0x00, &p)) {
		print("[ERROR] : Could not read disk.\r\n");
		goto error;
	}
	for(entry=(entry_t*)buffer; entry->filename[0] != 0x00; ++entry)
		switch((unsigned char)entry->filename[0]) {
		case 0xe5:
			puts("File deleted.");
		break;
		case 0x05:
			puts(entry->filename);
		break;
		case 0xff:
			puts(entry->filename);
		break;
		default:
		break;
		}

	/* start of actual command mode */
	puts("Press any key to continue...");
	getch();
	init_graphics(0x03);
	puts(INFOMSG);
	wait(1000000);
	beep();
	while(shell());
error:
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

