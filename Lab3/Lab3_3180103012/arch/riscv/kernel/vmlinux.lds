OUTPUT_ARCH( "riscv" )
ENTRY( _start )
BASE_ADDR = 0x80000000;
SECTIONS
{
    . = BASE_ADDR;
    .text : {
        *(.text.init)
        *(.text.entry)
        *(.text)
        }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(.bss) }
    . += 0x10fff;
    stack_top = 0x80010fff;
    _end = .;

    bss_size = 0x100;
    mtime_addr = 0x200bff8;
    mtimecmp_addr = 0x2004000;
    time_sep_init = 1000000;
    time_sep = 100000;
    mcause_MTimer = 0x7;
    mcause_ecallS = 0x9;
    scause_STimer = 0x5;
}
