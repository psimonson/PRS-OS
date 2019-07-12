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

#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x00"

/* Entry point for my command shell.
 */
void main()
{
	extern int shell();

	/* setup data segment */
	asm("push %cs");
	asm("pop %ds");

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

