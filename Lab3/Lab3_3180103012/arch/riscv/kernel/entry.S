.section .text.entry

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
            call do_timer
            ecall

            j trap_s_end

    trap_s_except:

        j trap_s_except_end  # No implementation yet

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

# t0 = &current
# t1 = &next
# t4 = THREAD_OFFSET
.global switch_to_asm
switch_to_asm:
    ld t2, 0(t0)    # t2 = current
    ld t3, 0(t1)    # t3 = next
    sd t3, 0(t0)    # current = next
    add t2, t2, t4  # t2 = &(current->thread)
    add t3, t3, t4  # t3 = &(next->thread)

    sd ra, 0(t2)
    sd sp, 8(t2)
    sd s0, 16(t2)
    sd s1, 24(t2)
    sd s2, 32(t2)
    sd s3, 40(t2)
    sd s4, 48(t2)
    sd s5, 56(t2)
    sd s6, 64(t2)
    sd s7, 72(t2)
    sd s8, 80(t2)
    sd s9, 88(t2)
    sd s10, 96(t2)
    sd s11, 104(t2)

    # ld ra, 0(t3)
    # ld sp, 8(t3)
    # ld s0, 16(t3)
    ld s1, 24(t3)
    ld s2, 32(t3)
    ld s3, 40(t3)
    ld s4, 48(t3)
    ld s5, 56(t3)
    ld s6, 64(t3)
    ld s7, 72(t3)
    ld s8, 80(t3)
    ld s9, 88(t3)
    ld s10, 96(t3)
    ld s11, 104(t3)

    ret