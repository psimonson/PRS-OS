/*
 * My very simple boot code, a base boot code.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc\n");
asm("jmpl $0, $main\n");

#include "disk.h"

#define IMAGE_SIZE 8192
#define BLOCK_SIZE 512
#define IMAGE_LMA  0x8000
#define IMAGE_ENTRY 0x800c

/* Simple print function for boot sector.
 */
void __REGPARM print(const char *s)
{
	while(*s) {
		asm volatile("int $0x10" : : "a"(0x0e00 | *s), "b"(7));
		s++;
	}
}
/* Entry point for boot sector.
 */
void __NORETURN main()
{
	drive_params_t p;
	unsigned char bios_drive = 0;
	void *buff = (void*)IMAGE_LMA;
	void *entry = (void*)IMAGE_ENTRY;
	unsigned short num_blocks = ((IMAGE_SIZE / BLOCK_SIZE) +
		 (IMAGE_SIZE % BLOCK_SIZE == 0 ? 0 : 1));

	asm volatile("movb %%dl, %0" : "=r"(bios_drive));
	get_drive_params(&p, bios_drive);
	asm(
		"xorw %ax, %ax\n"
		"movw %ax, %ds\n"
		"movw %ax, %es\n"
		"movw %ax, %fs\n"
		"movw %ax, %gs\n"
		"movw %ax, %ss\n"
		"mov $0x2000, %esp\n"
	);
	if(lba_read(buff, 1, num_blocks, bios_drive, &p) != 0) {
		print("read error :(\r\n");
		while(1);
	}
	print("Running next image...\r\n");
	asm volatile("" : : "d"(bios_drive));
	goto *entry;
}
