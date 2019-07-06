asm(".code16gcc\n");
asm("jmpl $0, $main\n");

#include "io.h"

void print_text()
{
	print("This is a simple bit of text\r\n:P\r\n");
}

void __NORETURN main()
{
	print_text();
	print("Hanging system.\r\n");
	while(1);
}
