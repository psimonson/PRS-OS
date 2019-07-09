/*
 * Simple implementations of string functions.
 * Author: Philip R. Simonson
 * Date  : July 8, 2019
 */

asm(".code16gcc\n");

#include "io.h"
#include "string.h"

/* My Simple implementation of gets.
 */
int gets(char *s, int size)
{
	char c;
	int i;

	for(i=0; (c = getche()) != -1 && c != '\r'; i++)
		s[i] = c;
	s[i] = '\0';
	return i;
}
/* Simple implementation of string compare.
 */
int strcmp(const char *s, const char *t)
{
	int i;

	for(i=0; s[i] != 0 || t[i] != 0; i++)
		if(s[i] != t[i])
			return s[i]-t[i];
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

		if(*begin == *pattern) while(*pattern && *begin == *pattern) begin++,pattern++;
		if(!*pattern) return begin;
		p++;
	}
	return 0;
}
