ENTRY(main);
SECTIONS
{
	. = 0x0000;
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
		*(.iplt*);
		*(.igot*);
		*(.rel*);
		*(.comment);
		*(.eh_frame);
	}
}
