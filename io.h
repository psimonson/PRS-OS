/*
 * Header for input/output using the BIOS interrupt.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

#ifndef IO_H
#define IO_H

#include "defines.h"

/* input/output functions */
int putch_color(int c, unsigned char color);
int putch(int c);
int print(const char *s);
int puts(const char *s);
void printlu(unsigned long n, char s[]);
char getch();
char getche();

/* graphics functions */
void init_graphics(unsigned char mode);
void putpixel(short y, short x, unsigned char color);

/* miscellaneous functions */
void set_cursoryx(char y, char x);
void reboot();

/* sound functions (PC Speaker) */
void play_sound(unsigned short nfreq);
void no_sound();
void beep();

/* cmos functions */
unsigned char cmos_read(unsigned char addr);
void cmos_write(unsigned char addr, unsigned char byte);
void cmos_invert(unsigned char addr);

/* input and output to ports functions */
void outb(unsigned short port, unsigned char byte);
unsigned char inb(unsigned short port);

#endif
