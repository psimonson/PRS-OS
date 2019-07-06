ENTRY(main);
SECTIONS
{
	. = 0x1000;
	.text :
	{
		*(.text);
	}
	.data :
	{
		*(.rodata);
	}
	/DISCARD/ :
	{
		*(.note*);
		*(.iplt*);
		*(.igot*);
		*(.rel*);
		*(.comment);
		*(.eh_frame);
	}
}
