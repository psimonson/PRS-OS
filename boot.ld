ENTRY(main);
SECTIONS
{
    . = 0x7C00;
    .text :
    {
        *(.text);
    }
    .data :
    {
        *(.rodata);
    }
    .sig : AT(0x7DFE)
    {
        SHORT(0xaa55);
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
