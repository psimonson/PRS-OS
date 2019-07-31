/*
 * Header for input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

#ifndef IO_H
#define IO_H

#include "defines.h"

#define NULL	((void*)0)	/* void pointer (null pointer) */
#define BUFSIZ	512		/* size of one sector on disk */

/* input/output functions */
int __REGPARM putch_color(int c, unsigned char color);
int __REGPARM putch(int c);
int __REGPARM printf(const char *format, ...);
int __REGPARM print(const char *s);
int __REGPARM puts(const char *s);
char __REGPARM getch();
char __REGPARM getche();

/* graphics functions */
void __REGPARM init_graphics(unsigned char mode);
void __REGPARM putpixel(short y, short x, unsigned char color);

/* miscellaneous functions */
void __REGPARM set_cursoryx(char y, char x);
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

#endif
