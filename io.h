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

#endif
