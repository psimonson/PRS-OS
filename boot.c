/*
 * My very simple boot code, a base boot code.
 * Author: Philip R. Simonson
 * Date  : July 5, 2019
 */

asm(".code16gcc\n");
asm("jmpl $0, $main\n");

#include "disk.h"

/*
#include "io.h"

void __NORETURN main()
{
	int i,x,y;

	init_graphics(0x0d);
	for(i=0; i<3; i++) {
		for(y=i*100; y<=110; y++)
			for(x=i*100; x<=110; x++)
				if((x%10) == 0 || (y%10) == 0)
				 	putpixel(y, x, 0x0a);
	}
	set_cursoryx(0x0e, 0x00);
	print("Press a key...\r\n");
	getch();
	init_graphics(0x03);
	while(1);
}
*/

#define IMAGE_SIZE 8192
#define BLOCK_SIZE 512
#define IMAGE_LMA  0x8000
#define IMAGE_ENTRY 0x800c

void __REGPARM print(const char *s)
{
	while(*s) {
		asm volatile("int $0x10" : : "a"(0x0e00 | *s), "b"(7));
		s++;
	}
}

void __NORETURN main()
{
	unsigned char bios_drive = 0;
	void *buff = (void*)IMAGE_LMA;
	void *entry = (void*)IMAGE_ENTRY;
	unsigned short num_blocks = ((IMAGE_SIZE / BLOCK_SIZE) +
		 ((IMAGE_SIZE % BLOCK_SIZE) == 0 ? 0 : 1));
	drive_params_t p;

	asm volatile("movb %%dl, %0" : "=r"(bios_drive));
	get_drive_params(&p, bios_drive);
	if(lba_read(buff, 1, num_blocks, bios_drive, &p) != 0) {
		print("read error :(\r\n");
		while(1);
	}
	print("Running next image...\r\n");
	asm volatile("" : : "d"(bios_drive));
	goto *entry;
}
