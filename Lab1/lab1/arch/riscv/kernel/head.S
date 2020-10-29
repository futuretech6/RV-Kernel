.text

.global _start
_start:
    # set mstatus, turn off mie and mipe
    li t1, 0x8
    csrrc x0, mstatus, t1

    # set mtvec(M mode异常处理地址)
    la t1, mtvec_base
    sll t1, t1, 2
    or t1, t1, 1  # mode = vectored
    csrrw x0, mtvec, t1

    # switch from M to S
    li t1, 0x1000  # MPP[1] = 0
    csrrc x0, mstatus, t1
    li t1, 0x800  # MPP[0] = 1
    csrrs x0, mstatus, t1
    la t1, S_Mode
    csrrw x0, mepc, t1
    mret

S_Mode:
    # set stvec(S mode异常处理地址)
    la t1, stvec_base
    sll t1, t1, 2
    or t1, t1, 1  # mode = vectored
    csrrw x0, stvec, t1
    
    # set sp
    la sp, stack_top

    # jump to start_kernel in main.c
    call start_kernel

.global _end
_end:
