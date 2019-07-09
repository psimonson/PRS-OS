/*
 * Simple input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");

#include "io.h"
#include "time.h"

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
/* Play a sound of nfreq.
 */
void __REGPARM play_sound(unsigned int nfreq)
{
	unsigned int div;
	unsigned char tmp;

	/* Set the PIT to the desired frequency */
	div = 1193180 / nfreq;
	outb(0x43, 0xB6);
	outb(0x42, (unsigned char)(div));
	outb(0x42, (unsigned char)(div >> 8));

	/* Play the sound */
	tmp = inb(0x61);
	if(tmp != (tmp | 3))
		outb(0x61, tmp | 3);
}
/* Stop sound.
 */
void __REGPARM no_sound()
{
	unsigned char tmp = inb(0x61) & 0xFC;
	outb(0x61, tmp);
}
/* Beep the PC speaker.
 */
void __REGPARM beep()
{
	play_sound(1000);
	wait(50000);
	no_sound();
}

/* --------------------------- Port Functions ------------------------- */

/* Output to port.
 */
void __REGPARM outb(unsigned short port, unsigned char value)
{
	asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}
/* Input to port.
 */
unsigned char __REGPARM inb(unsigned short port)
{
	unsigned char value = 0;
	asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

