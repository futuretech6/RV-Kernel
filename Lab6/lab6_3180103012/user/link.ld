ENTRY(_start)

SECTIONS
{
    . = 0x0;
    _start = .;
    .text.init : { KEEP(*(.text.init)) }
    .text : { KEEP(*(.text)) }
    .data : { KEEP(*(.data)) }
}
