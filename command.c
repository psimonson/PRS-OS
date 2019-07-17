/*
 * Main program for my boot code, just displays some graphics.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc");
/* setup segment registers */
asm(
	"movw %cs, %ax\n\t"
	"movw %ax, %ds\n\t"
	"movw %ax, %es\n\t"
	"movw %ax, %fs\n\t"
	"movw %ax, %gs\n\t"
	"cli\n\t"
	"movw %ax, %ss\n\t"
	"movw $0x0000, %sp\n\t"
	"sti\n\t"
);
asm("jmp main");

#include "io.h"
#include "string.h"
#include "time.h"
/*#include "fat12.h"*/

#define INFOMSG "\x43\x4f\x44\x45\x44\x20\x42\x59\x20\x50\x48\x49\x4c\x49\x50\x00"

/* Entry point for my command shell.
 */
void main()
{
	extern int shell();
/*	static boot_t _bs;
	int i;

	load_boot(&_bs);
	print("========================================================\r\n");
	puts("                   .:[BOOT SECTOR]:.");
	print("========================================================\r\n");
	for(i=0; i<sizeof(boot_t); i++) {
		if(i>0 && !(i%60))
			print("\r\n");
		putch(((unsigned char *)&_bs)[i]);
	}
	print("\r\n");
	print("========================================================\r\n");
*/
	/* start of actual command mode */
	printf("Press any key to continue...");
	getch();
	init_graphics(0x03);
	puts(INFOMSG);
	wait(1000000);
	beep();
	while(shell());
	puts("Hanging system.");
	asm("cli");
	asm("hlt");
}

