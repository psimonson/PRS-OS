/*
 * Main program for my boot code, just runs a simple shell.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");
asm("jmp main");

#include "io.h"
#include "string.h"
#include "time.h"
#include "fat12.h"
#include "disk.h"

/* my nice header */
#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x00"

/* boot sector */
boot_t *_boot_sector;
drive_params_t _drive_params;
void *_FAT_table;

/* external linkage; shell.c has this in it */
extern int shell();

/* Entry point for my command shell.
 */
void __REGPARM main()
{
	/* setup segment registers */
	asm(
		"cli\n\t"
		"movw $0x0050, %ax\n\t"
		"movw %ax, %ds\n\t"
		"movw %ax, %es\n\t"
		"movw %ax, %ss\n\t"
		"movw $0xffff, %sp\n\t"
		"sti\n\t"
	);

	/* get first floppy drive */
	if(get_drive_params(&_drive_params, 0))
		goto end;
	/* load boot sector into memory */
	if((_boot_sector = load_boot(&_drive_params)) == 0)
		goto end;
	/* load reserved FAT table */
	if((_FAT_table = load_fat12(&_drive_params)) == 0)
		goto end;

	/* start of actual command mode */
	printf("Press any key to continue...");
	getch();
	init_graphics(0x03);
	puts(INFOMSG);
	wait(1000000);
	beep();
	shell();

end:
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

