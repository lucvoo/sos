ENTRY(__exception_handlers)
GROUP(libtarget.a libgcc.a)


MEMORY
{
    sdram : ORIGIN = 0x100000, LENGTH = 0x7F0000 - 0x010000
}

SECTIONS
{
    .note.arm.ident 0 : { KEEP(*(.note.arm.ident)) }
    .rom_vectors 0x100000 : {
	PROVIDE(__rom_vectors_vma = ABSOLUTE(.));
	KEEP(*(.vectors))
    } > sdram
    PROVIDE(__rom_vectors_lma = LOADADDR(.rom_vectors));

    .text ALIGN (0x4) : {
	PROVIDE(__stext = ABSOLUTE(.));
	*(.text*)
    } > sdram
    PROVIDE(__etext = .);

    .rodata ALIGN (0x4) : {
	PROVIDE(__srodata = ABSOLUTE(.));
	*(.rodata*)

	. = ALIGN (4);
	PROVIDE(__INITCALL_LIST__ = ABSOLUTE(.));
	KEEP(*(SORT(.initcall*)))
	LONG(0)
	PROVIDE(__INITCALL_END__ = ABSOLUTE(.));
    } > sdram
    PROVIDE(__erodata = .);

    .data ALIGN (0x4) : {
	*(.data*)
    } > sdram
    PROVIDE(_edata = .);

    .bss ALIGN (0x4) : {
	PROVIDE(__bss_start = ABSOLUTE(.));
	*(.bss*) *(COMMON)
     } > sdram
    PROVIDE(__bss_end = ABSOLUTE(.));

    PROVIDE(__heap = ALIGN(0x8));
}
