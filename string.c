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
/* Simple implementation of itoa.
 */
void itoa(unsigned long n, char s[])
{
	int i,sign;
	if((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while((n /= 10) > 0);
	if(sign < 0)
		s[i++] = '-';
	s[i] = 0;
	reverse(s);
}
/* Simple implementation of string length.
 */
int strlen(const char *s)
{
	const char *p = s;
	while(*p) p++;
	return p-s;
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
/* Simple implementation of strchr.
 */
int strchr(char *s, const char c)
{
	char *p = s;
	while(*p)
		if(*p == c)
			return p-s;
	return 0;
}
/* Simple implementation of memset.
 */
int memset(void *mem, int c, int size)
{
	void *p = mem;
	while((unsigned long*)p-(unsigned long*)mem < size)
		*(unsigned long*)p++ = c;
	while((unsigned char*)p-(unsigned char*)mem < size)
		*(unsigned char*)p++ = c;
	return ((unsigned char*)p-(unsigned char*)mem);
}
/* Simple implementation of reverse.
 */
void reverse(char s[])
{
	int i,j;
	for(i=0,j=strlen(s)-1; i<j; i++,j--) {
		char tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}
}

