/*
 * Main program for my boot code, just displays some graphics.
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

#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x00"

extern int shell();

/* boot sector */
boot_t *_boot_sector;
drive_params_t p;
/*entry_t *_entry;
*/
/* Entry point for my command shell.
 */
void main()
{
	if(get_drive_params(&p, 0))
		goto end;
	if((_boot_sector = load_boot(&p)) == 0)
		goto end;
/*	_entry = load_root(&p, _boot_sector);
	printf("%d\r\n", _entry[0].filename[0]);
*/
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

