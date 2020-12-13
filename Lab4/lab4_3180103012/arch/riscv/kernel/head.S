.section .text.init

.global _start
_start:
    # turn off MMU
    csrw satp, zero

    # init .bss
    la t0, .bss  # pointer
    la t1, _end  # end
    bss_init_loop:
        sb zero, 0(t0)
        addi t0, t0, 1
        bne t0, t1, bss_init_loop

    # set mtimecmp to mtime+time_sep
    la t0, mtime_addr  # RV64, t0 is of 64 bit
    ld t1, 0(t0)       # mtime->t1
    la t0, time_sep_init
    add t1, t1, t0
    la t0, mtimecmp_addr
    sd t1, 0(t0)

    # set time int delegation
    li t1, 0x20  # mi[e|p][5]=sti[e|p]
    csrs mideleg, t1

    # set delegation of instruction/load/store page fault
    la t1, medeleg_bit
    csrs medeleg, t1  # just set is ok

    # set mstatus.mpie
    li t1, 0x80  # mstatus[7]
    csrs mstatus, t1

    # set mstatus.mie, mie.mtie, sstatus.sie, sie.stie
    li t1, 0x8   # mstatus[3]
    csrs mstatus, t1
    li t1, 0x80  # mie[7]
    csrs mie, t1
    li t1, 0x2   # sstatus[1]
    csrs sstatus, t1
    li t1, 0x20  # sie[5]
    csrs sie, t1

    # set mtvec(M mode)
    la t1, trap_m  # mode = directed
    csrw mtvec, t1

    # set mscratch to stack_top in physical space
    la t1, stack_top
    csrw mscratch, t1

    # switch from M to S
    li t1, 0x1000  # MPP[1] = 0
    csrc mstatus, t1
    li t1, 0x800   # MPP[0] = 1
    csrs mstatus, t1

    la t1, S_Mode
    csrw mepc, t1
    mret

S_Mode:
    # set stvec(physical)
    la t1, trap_s   # mode = directed
    csrw stvec, t1

    # set sp for `paging_init()` in Physical Space !!!!!
    la sp, init_stack_top

    # init page mapping **using physical address**
    call paging_init

    # set satp to turn on MMU
    ori t0, zero, 8  # mode=satp[63:60] = 8
    sll t0, t0, 16
    ori t0, t0, 0    # ASID=satp[59:44] = 0
    sll t0, t0, 44
    la t1, rt_pg_addr
    srl t1, t1, 12
    or t0, t0, t1    # PPN=satp[43:0] = 0x80000000 >> 12 = 0x80000, in unit 4KB(>>12)
    csrw satp, t0

    # refresh related cache
    sfence.vma

    # load phy2vir_diff to t2
    li t2, 0xffffffdf80000000

    # set stvec(virtual)
    la t1, trap_s   # mode = directed
    add t1, t1, t2  # convert to virtual
    csrw stvec, t1

    # set sp to init_stack_top in virtual space
    la sp, init_stack_top
    add sp, sp, t2  # convert to virtual

    # jump to start_kernel in virtual space
    la t1, start_kernel
    add t1, t1, t2  # convert to virtual
    jr t1


.global _end
_end:
