0000000000000004 <main>:
   4:   fd010113                addi    sp,sp,-48
   8:   02113423                sd      ra,40(sp)
   c:   02813023                sd      s0,32(sp)
  10:   00913c23                sd      s1,24(sp)
  14:   01213823                sd      s2,16(sp)
  18:   01313423                sd      s3,8(sp)
  1c:   00010993                mv      s3,sp
  20:   00000917                auipc   s2,0x0
  24:   3b890913                addi    s2,s2,952 # 3d8 <printf+0x358>
  28:   fff00493                li      s1,-1
  2c:   0ac00893                li      a7,172
  30:   00000073                ecall
  34:   00050413                mv      s0,a0
  38:   00098613                mv      a2,s3
  3c:   00040593                mv      a1,s0
  40:   00090513                mv      a0,s2
  44:   03c000ef                jal     ra,80 <printf>
  48:   00048793                mv      a5,s1
  4c:   fff7879b                addiw   a5,a5,-1
  50:   fe079ee3                bnez    a5,4c <main+0x48>
  54:   fd9ff06f                j       2c <main+0x28>

000000000000004c <printf>:
  ...
 380:   04000893                li      a7,64
 384:   00078513                li      a0,1
 388:   00070593                mv      a1,a4 # buf
 38c:   00068613                mv      a2,a3 # count
 390:   00000073                ecall