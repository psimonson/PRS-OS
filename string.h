/*
 * Simple implementation of string functions.
 * Author: Philip R. Simonson
 * Date  : July 8, 2019
 */

#ifndef STRING_H
#define STRING_H

#include "defines.h"

__REGPARM int gets(char *s, int size);
__REGPARM void itoh(unsigned long n, char s[]);
__REGPARM int atoi(const char *s);
__REGPARM void itoa(int n, char s[]);
__REGPARM void ltoa(long n, char s[]);
__REGPARM int strlen(const char *s);
__REGPARM int strcmp(const char *s, const char *t);
__REGPARM int memcmp(const void *m, const void *p, unsigned long nmem);
__REGPARM char *strstr(char *s, const char *t);
__REGPARM int strchr(char *s, const char c);
__REGPARM int strcpy(char *s, const char *t, int size);
__REGPARM int memset(void *mem, int c, int size);
__REGPARM int memcpy(void *dst, const void *src, int size);
__REGPARM void reverse(char s[]);

#endif
