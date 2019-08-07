/*
 * Simple implementation of string functions.
 * Author: Philip R. Simonson
 * Date  : July 8, 2019
 */

#ifndef STRING_H
#define STRING_H

int __REGPARM gets(char *s, int size);
void __REGPARM itoh(unsigned long n, char s[]);
int __REGPARM atoi(const char *s);
void __REGPARM itoa(int n, char s[]);
void __REGPARM ltoa(long n, char s[]);
int __REGPARM strlen(const char *s);
int __REGPARM strcmp(const char *s, const char *t);
int __REGPARM memcmp(const void *m, const void *p, unsigned long nmem);
char *__REGPARM strstr(char *s, const char *t);
int __REGPARM strchr(char *s, const char c);
int __REGPARM strcpy(char *s, const char *t, int size);
int __REGPARM memset(void *mem, int c, int size);
int __REGPARM memcpy(void *dst, const void *src, int size);
void __REGPARM reverse(char s[]);

#endif
