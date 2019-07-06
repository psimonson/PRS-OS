/*
 * Simple input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc\n");

#include "io.h"

/* ----------------------- Input/Output Functions ----------------------- */

/* Puts a character on the screen.
 */
void __REGPARM putch_color(char c, unsigned char color)
{
	asm("int $0x10": : "a"((0x0e << 8) | c), "b"((0x00 << 8) | color), "c"(0x0001));
}
/* Puts a character on the screen with default color 0x07.
 */
void __REGPARM putch(char c)
{
	putch_color(c, 0x07);
}
/* Prints a string on the screen using putch.
 */
void __REGPARM print(const char *s)
{
	while(*s)
		putch(*s++);
}
/* Gets a character from the keyboard.
 */
char __REGPARM getch()
{
	char c;
	asm("int $0x16" : "=a"(c) : "a"(0x0000));
	return c;
}
/* Gets a character from the keyboard and echoes it on screen.
 */
char __REGPARM getche()
{
	char c;
	c = getch();
	putch(c);
	return c;
}

/* ------------------------- Graphics Functions ------------------------- */

/* Initilize graphics.
 */
void __REGPARM init_graphics(unsigned char mode)
{
	asm("int $0x10": : "a"(0x0000 | mode));
}
/* Plot a pixel at given (y,x) coords.
 */
void __REGPARM putpixel(short y, short x, unsigned char color)
{
	asm("int $0x10": : "a"(0x0c00 | color), "b"(0x0000), "c"(y), "d"(x));
}

/* ---------------------- Miscellaneous Functions ----------------------- */

/* Set cursor position.
 */
void __REGPARM set_cursoryx(char y, char x)
{
	asm("int $0x10": : "a"(0x0200), "b"(0x0000), "d"((y << 8) | x));
}

