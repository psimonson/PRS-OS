/*
 * Some attribute defines, short hand.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

#ifndef DEFINES_H
#define DEFINES_H

#define __NOINLINE __attribute__((noinline))
#define __REGPARM __attribute__((regparm(3)))
#define __PACKED __attribute__((packed))
#define __NORETURN __attribute__((noreturn))

#endif
