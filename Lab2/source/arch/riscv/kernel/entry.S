.section .text.entry

.global trap_m
trap_m:
    # Save regs
    addi sp, sp, -24
    sd t0, 0(sp)
    sd t1, 8(sp)
    csrr t0, mcause
    sd t0, 16(sp)

    csrr t0, mcause
    srl t1, t0, 63    # MSB
    and t0, t0, 0xff  # trim MSB
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
            # set mtimecmp += time_sep, hardware will clear mip.mtip
            la t0, mtimecmp_addr
            ld t0, 0(t0)
            la t1, time_sep
            add t1, t0, t1
            la t0, mtimecmp_addr
            sd t1, 0(t0)

            
    li t1, 0x8   # mstatus[3]
    csrs mstatus, t1

            # enable mie.mtie
            li t0, 0x80
            csrs mie, t0
            

            j trap_m_except_end

        trap_m_except_end:
            csrr t0, mepc
            addi t0, t0, 4
            csrw mepc, t0

    trap_m_end:
    # Get regs back
    ld t0, 16(sp)
    csrw mcause, t0
    ld t1, 8(sp)
    ld t0, 0(sp)
    addi sp, sp, 24
    mret


.global trap_s
trap_s:
    # Save regs
    addi sp, sp, -24
    sd t0, 0(sp)
    sd t1, 8(sp)
    csrr t0, scause
    sd t0, 16(sp)

    csrr t0, scause
    srl t1, t0, 63    # MSB
    and t0, t0, 0xff  # trim MSB
    beq t1, zero, trap_s_except

    trap_s_int:
        la t1, scause_STimer
        beq t0, t1, trap_s_timer
        j trap_s_end

        trap_s_timer:
            call put_trap_s
            # enable sip.stip
            li t0, 0x20   # sip[5]
            csrc sip, t0  # Clear sip.stip means finish

            ecall

            j trap_s_end

    trap_s_except:
 
        j trap_s_end

        trap_s_except_end:
            csrr t0, sepc
            addi t0, t0, 4
            csrw sepc, t0

    trap_s_end:
    # Get regs back
    ld t0, 16(sp)
    csrw scause, t0
    ld t1, 8(sp)
    ld t0, 0(sp)
    addi sp, sp, 24
    sret
