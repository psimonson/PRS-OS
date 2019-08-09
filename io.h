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
int __REGPARM putch_color(int c, unsigned char color);
int __REGPARM putchar(int c, unsigned char delay);
int __REGPARM vprintf(const char *format, va_list ap, unsigned char delay);
int __REGPARM printf(const char *format, ...);
int __REGPARM typerf(const char *format, ...);
int __REGPARM print_delay(const char *s, unsigned char delay);
int __REGPARM puts(const char *s);
int __REGPARM getch();
int __REGPARM getche();

/* graphics functions */
void __REGPARM init_graphics(unsigned char mode);
void __REGPARM put_pixel(unsigned short y, unsigned short x, unsigned char color);

/* miscellaneous functions */
void __REGPARM set_cursor(unsigned short shape);
void __REGPARM set_cursoryx(unsigned char y, unsigned char x);
void __REGPARM reboot();

/* sound functions (PC Speaker) */
void __REGPARM play_sound(unsigned short nfreq);
void __REGPARM no_sound();
void __REGPARM beep();

/* cmos functions */
unsigned char __REGPARM cmos_read(unsigned char addr);
void __REGPARM cmos_write(unsigned char addr, unsigned char byte);
void __REGPARM cmos_invert(unsigned char addr);

/* input and output to ports functions */
void __REGPARM outb(unsigned short port, unsigned char byte);
unsigned char __REGPARM inb(unsigned short port);

#define putch(ch) putchar(ch, FALSE)
#define print(str) print_delay(str, FALSE)

#endif
