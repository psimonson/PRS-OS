/*
 * Simple input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");

#include "io.h"
#include "time.h"
#include "string.h"

/* ----------------------- Input/Output Functions ----------------------- */

/* Puts a character on the screen.
 */
int __REGPARM putch_color(int ic, unsigned char color)
{
	char c = (char)ic;
	asm("int $0x10": : "a"((0x0e << 8) | c), "b"((0x00 << 8) | color), "c"(0x0001));
	return c;
}
/* Puts a character on the screen with default color 0x07.
 */
int __REGPARM putchar(int ch, unsigned char delay)
{
	char c = (char)ch;
	if(delay) {
		play_sound(400);
		wait(50000);
		no_sound();
		wait(100000);
	}
	putch_color(c, 0x07);
	return ch;
}
/* Prints a formatted string on the screen using putch.
 */
int __REGPARM vprintf(const char *format, va_list ap, unsigned char delay)
{
	int i = 0;
	while(*format) {
		if(format[0] == '%' && format[1] == '%') {
			putchar(*++format, delay);
			i++;
		} else if(format[0] == '%' && format[1] != '%') {
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
				i += print_delay(buf, delay);
			break;
			case 'l':
				switch(*++format) {
				case 'd':
					l = (long)va_arg(ap, long);
					memset(buf, 0, sizeof(buf));
					ltoa(l, buf);
					i += print_delay(buf, delay);
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
				putchar(c, delay);
				i++;
			break;
			case 'x':
			case 'X':
				ul = (unsigned long)va_arg(ap, unsigned long);
				memset(buf, 0, sizeof(buf));
				itoh(ul, buf);
				i += print_delay(buf, delay);
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
				i += print_delay(buf, delay);
			break;
			case 's':
				s = (const char *)va_arg(ap, const char *);
				i += print_delay(s, delay);
			break;
			default:
			break;
			}
		} else {
			putchar(*format, delay);
			i++;
		}
		format++;
	}
	return i;
}
/* Prints a formatted string.
 */
int __REGPARM printf(const char *format, ...)
{
	int bytes = 0;
	va_list ap;
	va_start(ap, format);
	bytes = vprintf(format, ap, FALSE);
	va_end(ap);
	return bytes;
}
/* Text typer; prints a formatted string a character at a time with delay.
 */
int __REGPARM typerf(const char *format, ...)
{
	int bytes = 0;
	va_list ap;
	va_start(ap, format);
	bytes = vprintf(format, ap, TRUE);
	va_end(ap);
	return bytes;
}
/* Prints a string on the screen using putch.
 */
int __REGPARM print_delay(const char *s, unsigned char delay)
{
	const char *p = s;
	while(*p)
		putchar(*p++, delay);
	return p-s;
}
/* Prints a string with a new line.
 */
int __REGPARM puts(const char *s)
{
	return printf("%s\r\n", s);
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
/* Reboot system.
 */
void __REGPARM reboot()
{
	asm("jmpw $0xFFFF, $0x0000");
}

/* -------------------------- Sound Functions --------------------------- */

/* Play a sound of nfreq.
 */
void __REGPARM play_sound(unsigned short nfreq)
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

/* --------------------------- CMOS Functions ------------------------- */

/* Read a byte from the CMOS.
 */
unsigned char __REGPARM cmos_read(unsigned char addr)
{
	outb(0x70, addr);
	wait(50000);
	return inb(0x71);
}
/* Write a byte to the CMOS.
 */
void __REGPARM cmos_write(unsigned char addr, unsigned char byte)
{
	outb(0x70, addr);
	wait(50000);
	outb(0x71, byte);
}
/* Invert byte from CMOS.
 */
void __REGPARM cmos_invert(unsigned char addr)
{
	cmos_write(addr, 255 ^ cmos_read(addr));
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

