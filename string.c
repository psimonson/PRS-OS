/*
 * Simple implementations of string functions.
 * Author: Philip R. Simonson
 * Date  : July 8, 2019
 */

asm(".code16gcc");

#include "io.h"
#include "string.h"

/* My Simple implementation of gets.
 */
int gets(char *s, int size)
{
	char c;
	int i;

	for(i=0; (c = getch()) != -1 && c != '\r';)
		if(c == '\b') {
			if(i > 0) {
				s[--i] = 0;
				putch(c);
				putch(0);
				putch(c);
			}
		} else {
			s[i++] = c;
			putch(c);
		}
	s[i] = 0;
	return i;
}
/* Simple implementation of convert string to int.
 */
int atoi(const char *s)
{
	int number = 0;
	while(*s) {
		if(*s < '0' || *s > '9')
			return 0;
		number = (number << 3) + (number << 1) + *s-'0';
		s++;
	}
	return number;
}
/* Simple implementation of string compare.
 */
int strcmp(const char *s, const char *t)
{
	while(*s && *s == *t)
		s++, t++;
	return *s-*t;
}
/* Simple implementation of sub string.
 */
char *strstr(char *s, const char *t)
{
	char *p = s;
	while(*p) {
		char *begin = p;
		const char *pattern = t;

		while(*pattern && *begin == *pattern) begin++,pattern++;
		if(!*pattern) return begin;
		p++;
	}
	return 0;
}
