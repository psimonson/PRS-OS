/*
 * Header for input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

#ifndef IO_H
#define IO_H

#include "defines.h"

/* input/output functions */
void __NOINLINE __REGPARM putch_color(char c, unsigned char color);
void __NOINLINE __REGPARM putch(char c);
void __NOINLINE __REGPARM print(const char *s);

char __NOINLINE __REGPARM getch();
char __NOINLINE __REGPARM getche();

/* graphics functions */
void __NOINLINE init_graphics(unsigned char mode);
void __NOINLINE putpixel(int y, int x, unsigned char color);

#endif
