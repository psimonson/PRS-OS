/*
 * Simple implementation of string functions.
 * Author: Philip R. Simonson
 * Date  : July 8, 2019
 */

#ifndef STRING_H
#define STRING_H

int gets(char *s, int size);
int atoi(const char *s);
int strlen(const char *s);
int strcmp(const char *s, const char *t);
char *strstr(char *s, const char *t);
int strchr(char *s, const char c);
int memset(void *mem, int c, int size);
void reverse(char s[]);

#endif
