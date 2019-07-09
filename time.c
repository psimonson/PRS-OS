/*
 * Time function implementations, everything for timing/waiting.
 * Author: Philip R. Simonson
 * Date  : July 6, 2019
 */

#include "time.h"

/* Pause system for some amount of time.
 */
int wait(unsigned int ms)
{
	unsigned char failed = 0;
	asm volatile(
		"int $0x15\n"
		"setcb %0\n"
		: "=m"(failed)
		: "a"(0x8600), "c"((ms >> 24) | (ms >> 16)), "d"((ms >> 8) | ms)
	);
	return failed;
}
