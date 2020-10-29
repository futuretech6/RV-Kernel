OUTPUT_ARCH( "riscv" )
ENTRY( _start )
BASE_ADDR = 0x80000000;
SECTIONS
{
  . = BASE_ADDR;
  .text : { *(.text) }
  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss : { *(.bss) }
  . += 0x8000;
  stack_top = .;
  _end = .;
  mtvec_base = 0x0;
  stvec_base = 0x0;
}
