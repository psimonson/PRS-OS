/*
 * Main program for my boot code, just displays some graphics.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");
asm("jmp main");

#include "io.h"
#include "time.h"
#include "disk.h"

#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x00"

/*static boot_t const *_bs = (boot_t*)0x7c00;*/
static FILE *_disk = (FILE*)0x7e00;

/* Entry point for my command shell.
 */
void main()
{
	extern int shell();

	asm("push %cs");
	asm("pop %ds");

	/* read disk drive information into memory at 7e00 */
	asm("int $0x13" : "=c"(_disk->sectors) : "a"(0x0800), "d"(0x00) : "bx");
	_disk->sectors &= 0x3F;

	/* read the root directory */
	_buffer = (unsigned char*) 0x0200;
/*	_disk->lba = _bs->reserved_sectors + (_bs->fats * _bs->sectors_per_fat);
	_size = _bs->root_entries * sizeof(entry_t) / _bs->bytes_per_sector;
	if(read()) {
		print("Failed to read root directory.\r\n");
		goto error;
	}*/

	/* start of actual command mode */
	puts("Press any key to continue...");
	getch();
	init_graphics(0x03);
	puts(INFOMSG);
	wait(1000000);
	beep();
	while(shell());
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

