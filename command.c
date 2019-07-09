/*
 * Main program for my boot code, just displays some graphics.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");
asm("jmp main");

#include "io.h"
#include "time.h"

#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x0a\x0d"

extern int __REGPARM shell();

void __REGPARM graphics()
{
	int i,j;
	getch();
	init_graphics(0x0d);
	for(i=0; i < 100; i++) {
		for(j=0; j < 100; j++)
			putpixel(i, j, 0x0a);
	}
	set_cursoryx(0x0e, 0x00);
	print("Press a key to continue...\r\n");
	getch();
	init_graphics(0x03);
}

void __NORETURN main()
{
	asm(
		"push %cs\n"
		"pop %ds\n"
		"pop %es\n"
		"pop %fs\n"
		"pop %gs\n"
		"xorw %ax, %ax\n"
		"movw %ax, %ss\n"
		"movw $0xFFFF, %sp\n"
	);
	print("Press any key to continue...\r\n");
	graphics();
	print(INFOMSG);
	wait(1000000);
	beep();
	while(shell());
	print("Hanging system.\r\n");
	while(1);
}


