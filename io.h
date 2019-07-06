/*
 * Header for input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

#ifndef IO_H
#define IO_H

#include "defines.h"

/* input/output functions */
void __REGPARM putch_color(char c, unsigned char color);
void __REGPARM putch(char c);
void __REGPARM print(const char *s);

char __REGPARM getch();
char __REGPARM getche();

/* graphics functions */
void __REGPARM init_graphics(unsigned char mode);
void __REGPARM putpixel(short y, short x, unsigned char color);

/* miscellaneous functions */
void __REGPARM set_cursoryx(char y, char x);
void __REGPARM play_sound(unsigned int nfreq);
void __REGPARM no_sound();
void __REGPARM beep();

/* input and output to ports functions */
void __REGPARM outb(unsigned short port, unsigned char byte);
unsigned char __REGPARM inb(unsigned short port);

#endif
