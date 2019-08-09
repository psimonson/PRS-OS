/*
 * Header for input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

#ifndef IO_H
#define IO_H

#include "defines.h"
#include <stdarg.h>

#define TRUE		1		/* true 1=on */
#define FALSE		0		/* false 0=off */

#define NULL		((void*)0)	/* void pointer (null pointer) */
#define BUFSIZ		512		/* size of one sector on disk */

#define CSHAPE_NORM	0x0607		/* Normal cursor */
#define CSHAPE_BLOCK	0x0007		/* Block cursor */

/* input/output functions */
__REGPARM int putch_color(int c, unsigned char color);
__REGPARM int putchar(int c, unsigned char delay);
__REGPARM int vprintf(const char *format, va_list ap, unsigned char delay);
__REGPARM int printf(const char *format, ...);
__REGPARM int typerf(const char *format, ...);
__REGPARM int print_delay(const char *s, unsigned char delay);
__REGPARM int puts(const char *s);
__REGPARM int getch();
__REGPARM int getche();

/* graphics functions */
__REGPARM void init_graphics(unsigned char mode);
__REGPARM void put_pixel(unsigned short y, unsigned short x, unsigned char color);

/* miscellaneous functions */
__REGPARM void set_cursor(unsigned short shape);
__REGPARM void set_cursoryx(unsigned char y, unsigned char x);
__REGPARM void reboot();

/* sound functions (PC Speaker) */
__REGPARM void play_sound(unsigned short nfreq);
__REGPARM void no_sound();
__REGPARM void beep();

/* cmos functions */
__REGPARM unsigned char cmos_read(unsigned char addr);
__REGPARM void cmos_write(unsigned char addr, unsigned char byte);
__REGPARM void cmos_invert(unsigned char addr);

/* input and output to ports functions */
__REGPARM void outb(unsigned short port, unsigned char byte);
__REGPARM unsigned char inb(unsigned short port);

#define putch(ch) putchar(ch, FALSE)
#define print(str) print_delay(str, FALSE)

#endif
