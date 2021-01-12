OUTPUT_ARCH( "riscv" )
ENTRY( _start )
MEMORY {
    ram (wxa!ri) : ORIGIN = 0x0000000080000000, LENGTH = 16M
    ramv (wxa!ri) : ORIGIN = 0xffffffe000000000, LENGTH = 4096M
}
PHDRS {
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
    bss PT_LOAD;
}
SECTIONS {
    . = 0xffffffe000000000;
    .text : {
        PROVIDE(text_start = .);
        *(.text.init)
        *(.text.entry)
        *(.text .text.*)
        PROVIDE(text_end = .);
    } >ramv AT>ram :text
    .rodata : ALIGN(0x1000) {
        PROVIDE(rodata_start = .);
        . = ALIGN(16);
        *(.srodata .srodata.*)
        . = ALIGN(16);
        *(.rodata .rodata.*)
        PROVIDE(rodata_end = .);
    } >ramv AT>ram :rodata
    .data : ALIGN(0x1000) {
        PROVIDE(data_start = .);
        . = ALIGN(16);
        *(.sdata .sdata.*)
        . = ALIGN(16);
        *(.data .data.*)
        PROVIDE(data_end = .);
    } >ramv AT>ram :data
    .bss : ALIGN(0x1000) {
        PROVIDE(bss_start = .);
        . = ALIGN(16);
        *(.sbss .sbss.*)
        . = ALIGN(16);
        *(.bss .bss.*)
        PROVIDE(bss_end = .);
    } >ramv AT>ram :bss

    . = ALIGN(0x1000);
    . += 0x1000;
    init_stack_top = .;
    . += 0x1000;
    stack_top = .;
    _end = .;

    physical_base = 0x80000000;
    virtual_mask  = 0xffffff8000000000;
    virtual_base  = 0xffffffe000000000;
    phy2vir_diff  = 0xffffffdf80000000;

    kernel_rt_pg_addr = _end;
    kernel_rt_pg_addr = ALIGN(0x20000);

    mtime_addr    = 0x200bff8;
    mtimecmp_addr = 0x2004000;
    time_sep_init = 5000000;
    time_sep      = 5000000;

    mcause_i_MTimer  = 7;

    mcasue_e_inst    = 2;
    mcause_e_ldFault = 5;
    mcause_e_ecallS  = 9;
    mcause_e_loadPF  = 13;

    scause_i_STimer  = 5;

    scause_e_ecallU  = 8;
    scause_e_ecallS  = 9;
    scause_e_instPF  = 12;
    scause_e_loadPF  = 13;
    scause_e_storePF = 15;

    medeleg_bit = 0xB100;

    USER_PHY_ENTRY = 0x84000000;
}
