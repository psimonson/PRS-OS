ENTRY(main);
SECTIONS
{
	. = 0x0050;
	.text :
	{
		*(.text);
	}
	.data :
	{
		*(.bss);
		*(.rodata);
		*(.data);
	}
	/DISCARD/ :
	{
		*(.note);
		*(.comment);
	}
}
