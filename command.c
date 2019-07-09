/*
 * Main program for my boot code, just displays some graphics.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");
asm("jmp main");

#include "io.h"
#include "time.h"

#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x0a\x0d\x00"

void main()
{
	extern int shell();
	asm("push %cs");
	asm("pop %ds");
	print("Press any key to continue...\r\n");
	getch();
	init_graphics(0x03);
	print(INFOMSG);
	wait(1000000);
	beep();
	while(shell());
	print("Hanging system.\r\n");
	asm("cli");
	asm("hlt");
}


