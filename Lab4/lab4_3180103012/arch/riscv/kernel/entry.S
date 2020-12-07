.section .text.entry

.global thread_init
thread_init:
    la t0, dead_loop
    csrw sepc, t0
    sret

.global trap_m
trap_m:
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
    csrr t0, mcause
    sd t0, 8(sp)
    csrr t0, mepc
    sd t0, 0(sp)

    csrr t0, mcause
    srli t1, t0, 63    # t1 = MSB
    andi t0, t0, 0xff  # t0 = code
    beq t1, zero, trap_m_except

    trap_m_int:
        la t1, mcause_MTimer
        beq t0, t1, trap_m_timer
        j trap_m_end

        trap_m_timer:
            # enable mip.stip
            li t0, 0x20   # mip[5]
            csrs mip, t0  # For trap_s

            # clear mie.mtie
            li t0, 0x80
            csrc mie, t0

            j trap_m_end

    trap_m_except:
        la t1, mcause_ecallS
        beq t0, t1, trap_m_ecallS
        j trap_m_except_end

        trap_m_ecallS:
            # !!!!!! clear mip.stip !!!!!!
            li t0, 0x20   # sip[5]
            csrc mip, t0  # Stop from calling trap_s

            # set mtimecmp += time_sep, hardware will clear mip.mtip
            la t0, mtime_addr
            ld t0, 0(t0)    # t0 = mtimecmp
            la t1, time_sep
            add t1, t0, t1  # t1 = mtimecmp+sep
            la t0, mtimecmp_addr
            sd t1, 0(t0)

            # enable mie.mtie
            li t0, 0x80
            csrs mie, t0

            j trap_m_except_end

        trap_m_except_end:
            ld t0, 0(sp)
            addi t0, t0, 4  # mepc += 4
            sd t0, 0(sp)

    trap_m_end:
    # Get regs back
    ld t0, 0(sp)
    csrw mepc, t0
    ld t0, 8(sp)
    csrw mcause, t0
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

    mret


.global trap_s
trap_s:
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
    csrr t0, scause
    sd t0, 8(sp)
    csrr t0, sepc
    sd t0, 0(sp)

    csrr t0, scause
    srli t1, t0, 63    # t1 = MSB
    andi t0, t0, 0xff  # t0 = code
    beq t1, zero, trap_s_except

    trap_s_int:
        la t1, scause_STimer
        beq t0, t1, trap_s_timer
        j trap_s_end

        trap_s_timer:
            ecall
            call do_timer

            j trap_s_end

    trap_s_except:

        # No implementation yet
        j trap_s_except_end

        trap_s_except_end:
            ld t0, 0(sp)
            addi t0, t0, 4  # sepc += 4
            sd t0, 0(sp)

    trap_s_end:
    # Get regs back
    ld t0, 0(sp)
    csrw mepc, t0
    ld t0, 8(sp)
    csrw mcause, t0
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

    sret
