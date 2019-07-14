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

drive_params_t p;
boot_t bs;

/* Entry point for my command shell.
 */
void main()
{
	extern int shell();
	extern drive_params_t p;
	extern boot_t bs;
	int i;

	/* setup segment registers */
	asm("push %cs");
	asm("pop %ds");
	asm("pop %es");
	asm("pop %fs");
	asm("pop %gs");

	if(get_drive_params(&p, 0x00)) {
		print("Cannot get drive params.\r\n");
		goto disk_error;
	}
	if(read_drive(&bs, 1, 0, 0, 1, &p)) {
		print("Error: Cannot read boot sector of drive.\r\n");
		goto disk_error;
	}
	for(i=0; i<512; i++)
		putch(((unsigned char *)&bs)[i]);
	print("\r\n");

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

disk_error:
	get_drive_error(&p);
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

