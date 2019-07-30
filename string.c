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
/* Simple implementation of hex to string.
 */
void itoh(unsigned long n, char s[])
{
	const char hex[] = "0123456789ABCDEF";
	int i = 0;
	do {
		s[i++] = hex[(n % 16)];
	} while((n /= 16) > 0);
	s[i] = 0;
	reverse(s);
}
/* Simple implementation of convert string to int.
 */
int atoi(const char *s)
{
	int number = 0;
	while(*s) {
		if(*s < '0' || *s > '9')
			return 0;
		number += (*s - '0') * 10;
		s++;
	}
	return number;
}
/* Simple implementation of itoa.
 */
void itoa(int n, char s[])
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
/* Simple implementation of ltoa.
 */
void ltoa(long n, char s[])
{
	long i, sign;
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
/* Simple implementation of memcmp.
 */
int memcmp(const void *m, const void *p, unsigned long nmem)
{
	unsigned long i;
	for(i=0; i<nmem; i++)
		if(((unsigned char*)m)[i] != ((unsigned char *)p)[i])
			return ((unsigned char *)m)[i]-((unsigned char *)p)[i];
	return 0;
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
/* Simple implementation of strcpy.
 */
int strcpy(char *s, const char *t, int size)
{
	char *p = (char*)t;
	int i = 0;
	while(i < size && (*s = *p))
		s++,p++;
	*s = 0;
	return p-t;
}
/* Simple implementation of memset.
 */
int memset(void *mem, int c, int size)
{
	unsigned char *p = mem;
	while(p-((unsigned char*)mem) < size)
		*p++ = c;
	return (p-((unsigned char*)mem));
}
/* Simple implementation of memcpy.
 */
int memcpy(void *dst, const void *src, int size)
{
	char *cdst = (char*)dst;
	const char *csrc = (const char*)src;
	int i;
	for(i=0; i < size; i++)
		cdst[i] = csrc[i];
	return i;
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

