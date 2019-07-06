/*
 * Main program for my boot code, just displays some graphics.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc\n");
asm("jmpl $0, $main\n");

#include "io.h"

void __REGPARM graphics();

void main()
{
	print("Press any key to continue...\r\n");
	graphics();
	print("Hanging system.\r\n");
	while(1);
}

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

