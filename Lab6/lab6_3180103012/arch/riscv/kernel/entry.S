.section .text.entry

.global thread_init
thread_init:
    li sp, 0xffffffdf80000000
    li t0, 0x100
    csrc sstatus, t0  # sstatus.SPP=0
    li t0, 0x0
    csrw sepc, t0
    sret

.global trap_m
trap_m:
    # Exchange sp and mscratch to use physical stack
    csrrw sp, mscratch, sp

    # Save regs
    addi sp, sp, -264
    sd x1, 256(sp)
    sd x2, 248(sp)
    sd x3, 240(sp)
    sd x4, 232(sp)
    sd x5, 224(sp)
    sd x6, 216(sp)
    sd x7, 208(sp)
    sd x8, 200(sp)
    sd x9, 192(sp)
    sd x10, 184(sp)
    sd x11, 176(sp)
    sd x12, 168(sp)
    sd x13, 160(sp)
    sd x14, 152(sp)
    sd x15, 144(sp)
    sd x16, 136(sp)
    sd x17, 128(sp)
    sd x18, 120(sp)
    sd x19, 112(sp)
    sd x20, 104(sp)
    sd x21, 96(sp)
    sd x22, 88(sp)
    sd x23, 80(sp)
    sd x24, 72(sp)
    sd x25, 64(sp)
    sd x26, 56(sp)
    sd x27, 48(sp)
    sd x28, 40(sp)
    sd x29, 32(sp)
    sd x30, 24(sp)
    sd x31, 16(sp)
    csrr s0, mcause
    sd s0, 8(sp)
    csrr s0, mepc
    sd s0, 0(sp)

    # ================================ trap_m starts here ================================

    csrr s0, mcause
    srli s1, s0, 63    # s1 = MSB
    andi s0, s0, 0xff  # s0 = code
    beq s1, zero, trap_m_except

    trap_m_int:
        la s1, mcause_i_MTimer
        beq s0, s1, trap_m_timer
        j trap_m_end

        trap_m_timer:
            # enable mip.stip
            li s0, 0x20   # mip[5]
            csrs mip, s0  # For trap_s

            # clear mie.mtie
            li s0, 0x80
            csrc mie, s0

            j trap_m_end

    trap_m_except:
        la s1, mcause_e_ecallS
        beq s0, s1, trap_m_ecallS
        j trap_m_except_end

        trap_m_ecallS:
            # !!!!!! clear mip.stip !!!!!!
            li s0, 0x20   # sip[5]
            csrc mip, s0  # Stop from calling trap_s

            # set mtimecmp = mtime + time_sep, hardware will clear mip.mtip
            la s0, mtime_addr
            ld s0, 0(s0)    # s0 = mtime
            la s1, time_sep
            # li s1, 0x4000000
            add s1, s0, s1  # s1 = mtime+sep
            la s0, mtimecmp_addr
            sd s1, 0(s0)

            # enable mie.mtie
            li s0, 0x80
            csrs mie, s0

            j trap_m_except_end

        trap_m_except_end:
            ld s0, 0(sp)
            addi s0, s0, 4  # mepc += 4
            sd s0, 0(sp)

    trap_m_end:

    # ================================ trap_m ends here ================================

    # Get regs back
    ld s0, 0(sp)
    csrw mepc, s0
    ld s0, 8(sp)
    csrw mcause, s0
    ld x31, 16(sp)
    ld x30, 24(sp)
    ld x29, 32(sp)
    ld x28, 40(sp)
    ld x27, 48(sp)
    ld x26, 56(sp)
    ld x25, 64(sp)
    ld x24, 72(sp)
    ld x23, 80(sp)
    ld x22, 88(sp)
    ld x21, 96(sp)
    ld x20, 104(sp)
    ld x19, 112(sp)
    ld x18, 120(sp)
    ld x17, 128(sp)
    ld x16, 136(sp)
    ld x15, 144(sp)
    ld x14, 152(sp)
    ld x13, 160(sp)
    ld x12, 168(sp)
    ld x11, 176(sp)
    ld x10, 184(sp)
    ld x9, 192(sp)
    ld x8, 200(sp)
    ld x7, 208(sp)
    ld x6, 216(sp)
    ld x5, 224(sp)
    ld x4, 232(sp)
    ld x3, 240(sp)
    ld x2, 248(sp)
    ld x1, 256(sp)
    addi sp, sp, 264
    # Exchange sp and mscratch to use physical stack
    csrrw sp, mscratch, sp
    mret


.global trap_s
trap_s:
    # Exchange sp and mscratch to use physical stack
    # csrrw t3, sscratch, t3  # sp = addr of S sp, ssracth = U sp
    # ld sp, 0(t3)
    csrrw sp, sscratch, sp

    # Save regs
    addi sp, sp, -264
    sd x1, 256(sp)
    sd x2, 248(sp)
    sd x3, 240(sp)
    sd x4, 232(sp)
    sd x5, 224(sp)
    sd x6, 216(sp)
    sd x7, 208(sp)
    sd x8, 200(sp)
    sd x9, 192(sp)
    sd x10, 184(sp)
    sd x11, 176(sp)
    sd x12, 168(sp)
    sd x13, 160(sp)
    sd x14, 152(sp)
    sd x15, 144(sp)
    sd x16, 136(sp)
    sd x17, 128(sp)
    sd x18, 120(sp)
    sd x19, 112(sp)
    sd x20, 104(sp)
    sd x21, 96(sp)
    sd x22, 88(sp)
    sd x23, 80(sp)
    sd x24, 72(sp)
    sd x25, 64(sp)
    sd x26, 56(sp)
    sd x27, 48(sp)
    sd x28, 40(sp)
    sd x29, 32(sp)
    sd x30, 24(sp)
    sd x31, 16(sp)
    csrr s0, scause
    sd s0, 8(sp)
    csrr s0, sepc
    sd s0, 0(sp)

    # ================================ trap_s starts here ================================

    csrr s0, scause
    srli s1, s0, 63    # s1 = MSB
    andi s0, s0, 0xff  # s0 = code
    beq s1, zero, trap_s_except

    trap_s_int:
        la s1, scause_i_STimer
        beq s0, s1, trap_s_timer
        j trap_s_end

        trap_s_timer:
            ecall
            call do_timer
            j trap_s_end

    trap_s_except:

        la s1, scause_e_ecallU
        beq s0, s1, trap_s_ecallU
        la s1, scause_e_ecallS
        beq s0, s1, trap_s_instPF
        la s1, scause_e_loadPF
        beq s0, s1, trap_s_loadPF
        la s1, scause_e_storePF
        beq s0, s1, trap_s_storePF
        j trap_s_except_end

        trap_s_ecallU:
            addi sp, sp, -256
            sd x0, 0(sp)
            sd x1, 8(sp)
            sd x2, 16(sp)
            sd x3, 24(sp)
            sd x4, 32(sp)
            sd x5, 40(sp)
            sd x6, 48(sp)
            sd x7, 56(sp)
            sd x8, 64(sp)
            sd x9, 72(sp)
            sd x10, 80(sp)
            sd x11, 88(sp)
            sd x12, 96(sp)
            sd x13, 104(sp)
            sd x14, 112(sp)
            sd x15, 120(sp)
            sd x16, 128(sp)
            sd x17, 136(sp)
            sd x18, 144(sp)
            sd x19, 152(sp)
            sd x20, 160(sp)
            sd x21, 168(sp)
            sd x22, 176(sp)
            sd x23, 184(sp)
            sd x24, 192(sp)
            sd x25, 200(sp)
            sd x26, 208(sp)
            sd x27, 216(sp)
            sd x28, 224(sp)
            sd x29, 232(sp)
            sd x30, 240(sp)
            sd x31, 248(sp)

            csrr a0, scause  // a0 and a1 will be overwritten here
            csrr a1, sepc
            mv a2, sp
            call handler_s  // a0, a1 in stack have been overwrite
            ld a0, 80(sp)
            ld a1, 88(sp)   // Other regs will be loaded later
            addi sp, sp, 256
            j trap_s_except_end

        trap_s_instPF:
            call strap_instPF
            j trap_s_except_end

        trap_s_loadPF:
            call strap_loadPF
            j trap_s_except_end

        trap_s_storePF:
            call strap_storePF
            j trap_s_except_end

        trap_s_except_end:
            ld s0, 0(sp)
            addi s0, s0, 4  # sepc += 4
            sd s0, 0(sp)

    trap_s_end:

    # ================================ trap_s ends here ================================

    # Get regs back
    ld s0, 0(sp)
    csrw sepc, s0
    ld s0, 8(sp)
    csrw scause, s0
    ld x31, 16(sp)
    ld x30, 24(sp)
    ld x29, 32(sp)
    ld x28, 40(sp)
    ld x27, 48(sp)
    ld x26, 56(sp)
    ld x25, 64(sp)
    ld x24, 72(sp)
    ld x23, 80(sp)
    ld x22, 88(sp)
    ld x21, 96(sp)
    ld x20, 104(sp)
    ld x19, 112(sp)
    ld x18, 120(sp)
    ld x17, 128(sp)
    ld x16, 136(sp)
    ld x15, 144(sp)
    ld x14, 152(sp)
    ld x13, 160(sp)
    ld x12, 168(sp)
    # ld x11, 176(sp)  # a1
    # ld x10, 184(sp)  # a0
    ld x9, 192(sp)
    ld x8, 200(sp)
    ld x7, 208(sp)
    ld x6, 216(sp)
    ld x5, 224(sp)
    ld x4, 232(sp)
    ld x3, 240(sp)
    ld x2, 248(sp)
    ld x1, 256(sp)
    addi sp, sp, 264

    # Exchange sp and mscratch to use physical stack
    # sd sp, 0(t3)
    # csrrw t3, sscratch, t3
    csrrw sp, sscratch, sp
    sret
