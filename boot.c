/*
 * My very simple boot code, a base boot code.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc\n");
asm("jmpl $0, $main\n");

#include "io.h"

/* Entry point for boot loader.
 */
void __NORETURN main()
{
	int i,x,y;

	init_graphics(0x0d);
	for(i=0; i<3; i++) {
		for(y=i*100; y<=110; y++)
			for(x=i*100; x<=110; x++)
				if((x%10) == 0 || (y%10) == 0)
				 	putpixel(y, x, 0x0a);
	}
	set_cursoryx(0x0e, 0x00);
	print("Press a key...\r\n");
	getch();
	init_graphics(0x03);
	while(1);
}

