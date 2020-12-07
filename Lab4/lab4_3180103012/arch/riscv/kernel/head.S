.section .text.init

.global _start
_start:
    # init .bss
    la t0, .bss      # pointer
    la t1, bss_size  # counter
    bss_init_loop:
        sb zero, 0(t0)
        addi t0, t0, 1
        addi t1, t1, -1
        bne t1, zero, bss_init_loop

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

    # switch from M to S
    li t1, 0x1000  # MPP[1] = 0
    csrc mstatus, t1
    li t1, 0x800   # MPP[0] = 1
    csrs mstatus, t1

    la t1, S_Mode
    csrw mepc, t1
    mret

S_Mode:
    # set stvec(S mode)
    la t1, trap_s  # mode = directed
    csrw stvec, t1

    # set sp
    la sp, stack_top

    # jump to start_kernel in main.c
    call start_kernel

.global _end
_end:
