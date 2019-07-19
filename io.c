/*
 * Simple input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");

#include <stdarg.h>

#include "io.h"
#include "time.h"
#include "string.h"

/* ----------------------- Input/Output Functions ----------------------- */

/* Puts a character on the screen.
 */
int putch_color(int ic, unsigned char color)
{
	char c = (char)ic;
	asm("int $0x10": : "a"((0x0e << 8) | c), "b"((0x00 << 8) | color), "c"(0x0001));
	return c;
}
/* Puts a character on the screen with default color 0x07.
 */
int putch(int ic)
{
	char c = (char)ic;
	putch_color(c, 0x07);
	return ic;
}
/* Prints a formatted string on the screen using putch.
 */
int printf(const char *format, ...)
{
	va_list ap;
	int i = 0;

	va_start(ap, format);
	while(*format) {
		if(format[0] == '%' && format[1] == '%')
			putch('%');
		else if(format[0] == '%' && format[1] != '%'){
			unsigned long ul;
			const char *s;
			char buf[256];
			int c, d;
			void *p;
			long l;

			switch(*++format) {
			case 'd':
			case 'i':
				d = (int)va_arg(ap, int);
				memset(buf, 0, sizeof(buf));
				itoa(d, buf);
				print(buf);
				i += strlen(buf);
			break;
			case 'l':
				switch(*++format) {
				case 'd':
					l = (long)va_arg(ap, long);
					memset(buf, 0, sizeof(buf));
					ltoa(l, buf);
					print(buf);
					i += strlen(buf);
				break;
				case 'u':
					/* TODO: implement unsigned long */
				break;
				default:
				break;
				}
			break;
			case 'c':
				c = (int)va_arg(ap, int);
				putch(c);
				i++;
			break;
			case 'x':
			case 'X':
				ul = (unsigned long)va_arg(ap, unsigned long);
				memset(buf, 0, sizeof(buf));
				itoh(ul, buf);
				print(buf);
				i += strlen(buf);
			break;
			case 'p':
			case 'P':
				p = (void *)va_arg(ap, void *);
				if(p == 0) {
					printf("<null>\r\n");
					break;
				}
				memset(buf, 0, sizeof(buf));
				itoh((int)p, buf);
				print(buf);
				i += strlen(buf);
			break;
			case 's':
				s = (const char *)va_arg(ap, const char *);
				print(s);
				i += strlen(s);
			break;
			default:
			break;
			}
		} else {
			putch(*format);
			i++;
		}
		format++;
	}
	va_end(ap);
	return i;
}
/* Prints a string on the screen using putch.
 */
int print(const char *s)
{
	const char *p = s;
	while(*p)
		putch(*p++);
	return p-s;
}
/* Prints a string with a new line.
 */
int puts(const char *s)
{
	int bytes = 0;
	if((bytes = print(s)))
		return bytes += print("\r\n");
	return -1;
}
/* Gets a character from the keyboard.
 */
char getch()
{
	char c;
	asm("int $0x16" : "=a"(c) : "a"(0x0000));
	return c;
}
/* Gets a character from the keyboard and echoes it on screen.
 */
char getche()
{
	char c;
	c = getch();
	putch(c);
	return c;
}

/* ------------------------- Graphics Functions ------------------------- */

/* Initilize graphics.
 */
void init_graphics(unsigned char mode)
{
	asm("int $0x10": : "a"(0x0000 | mode));
}
/* Plot a pixel at given (y,x) coords.
 */
void putpixel(short y, short x, unsigned char color)
{
	asm("int $0x10": : "a"(0x0c00 | color), "b"(0x0000), "c"(y), "d"(x));
}

/* ---------------------- Miscellaneous Functions ----------------------- */

/* Set cursor position.
 */
void set_cursoryx(char y, char x)
{
	asm("int $0x10": : "a"(0x0200), "b"(0x0000), "d"((y << 8) | x));
}
/* Reboot system.
 */
void reboot()
{
	asm("jmpw $0xFFFF, $0x0000");
}

/* -------------------------- Sound Functions --------------------------- */

/* Play a sound of nfreq.
 */
void play_sound(unsigned short nfreq)
{
	unsigned short div;
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
void no_sound()
{
	unsigned char tmp = inb(0x61) & 0xFC;
	outb(0x61, tmp);
}
/* Beep the PC speaker.
 */
void beep()
{
	play_sound(1000);
	wait(50000);
	no_sound();
}

/* --------------------------- CMOS Functions ------------------------- */

/* Read a byte from the CMOS.
 */
unsigned char cmos_read(unsigned char addr)
{
	outb(0x70, addr);
	wait(50000);
	return inb(0x71);
}
/* Write a byte to the CMOS.
 */
void cmos_write(unsigned char addr, unsigned char byte)
{
	outb(0x70, addr);
	wait(50000);
	outb(0x71, byte);
}
/* Invert byte from CMOS.
 */
void cmos_invert(unsigned char addr)
{
	cmos_write(addr, 255 ^ cmos_read(addr));
}

/* --------------------------- Port Functions ------------------------- */

/* Output to port.
 */
void outb(unsigned short port, unsigned char value)
{
	asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}
/* Input to port.
 */
unsigned char inb(unsigned short port)
{
	unsigned char value = 0;
	asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

