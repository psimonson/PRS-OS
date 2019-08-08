/*
 * Some attribute defines, short hand.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

#ifndef DEFINES_H
#define DEFINES_H

#define STR_HELPER(X)	#X
#define STR(X)		STR_HELPER(X)

#define VERSION_MAJOR	0
#define VERSION_MINOR	1
#define PROGRAM_VERSION	"v" STR(VERSION_MAJOR) "." STR(VERSION_MINOR)

#define __NOINLINE	__attribute__((noinline))
#define __REGPARM	__attribute__((regparm(3)))
#define __PACKED	__attribute__((packed))
#define __NORETURN	__attribute__((noreturn))

#endif
