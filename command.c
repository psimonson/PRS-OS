/*
 * Main program for my boot code, just runs a simple shell.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");
/* setup segment registers */
asm(
	"movw %cs, %ax\n\t"
	"movw %ax, %ds\n\t"
	"movw %ax, %es\n\t"
	"movw %ax, %fs\n\t"
	"movw %ax, %gs\n\t"
	"cli\n\t"
	"movw %ax, %ss\n\t"
	"movw $0x0000, %sp\n\t"
	"sti\n\t"
);
asm("jmp main");

#include "io.h"
#include "string.h"
#include "time.h"
#include "fat12.h"
#include "disk.h"

/* my nice header */
#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x00"
/* enable testing for FAT12 filesystem */
#define TEST_FAT12 0	/* switch to 1 to enable, 0 disables it. */

/* external linkage; shell.c has this in it */
extern int shell();

/* boot sector */
boot_t *_boot_sector;
drive_params_t p;
#if TEST_FAT12
entry_t *_entry;
#endif

/* Entry point for my command shell.
 */
void main()
{
#if TEST_FAT12
	entry_t *file;
#endif
	/* get first floppy drive */
	if(get_drive_params(&p, 0))
		goto end;
	/* load boot sector into memory */
	if((_boot_sector = load_boot(&p)) == 0)
		goto end;

#if TEST_FAT12
	/* load root directory and list files */
	_entry = load_root(&p, _boot_sector);
	for(file=_entry; file->filename[0] != 0x00; ++file) {
		if(file->filename[0] == 0x41) {
			int i;
			for(i=1; i<11; i+=2)
				printf("%c", file->filename[i]);
			printf("\r\n");
		}
	}
#endif

/* quickly disable with #if 0 for testing something else. */
#if 1
	/* start of actual command mode */
	printf("Press any key to continue...");
	getch();
	init_graphics(0x03);
	puts(INFOMSG);
	wait(1000000);
	beep();
	shell();
#endif

end:
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

