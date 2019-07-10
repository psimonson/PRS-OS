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

drive_params_t p;

/* Entry point for my command shell.
 */
void main()
{
	extern drive_params_t p;
	extern int shell();

	asm("push %cs");
	asm("pop %ds");

	/* get current drive */
	if(get_drive_params(&p, 0x00)) {
		print("[ERROR] : Failed to get current drive info.\r\n");
		goto error;
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

