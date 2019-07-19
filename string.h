/*
 * Simple implementation of string functions.
 * Author: Philip R. Simonson
 * Date  : July 8, 2019
 */

#ifndef STRING_H
#define STRING_H

int gets(char *s, int size);
void itoh(unsigned long n, char s[]);
int atoi(const char *s);
void itoa(int n, char s[]);
void ltoa(long n, char s[]);
int strlen(const char *s);
int strcmp(const char *s, const char *t);
char *strstr(char *s, const char *t);
int strchr(char *s, const char c);
int strcpy(char *s, const char *t, int size);
int memset(void *mem, int c, int size);
int memcpy(void *dst, const void *src, int size);
void reverse(char s[]);

#endif
