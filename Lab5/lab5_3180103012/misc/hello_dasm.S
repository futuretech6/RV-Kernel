
/home/oslab/lab5/user/hello.elf:     file format elf64-littleriscv


Disassembly of section .text.init:

0000000000000000 <_start>:
   0:	0040006f          	j	4 <main>

Disassembly of section .text.main:

0000000000000004 <main>:
   4:	fd010113          	addi	sp,sp,-48
   8:	02113423          	sd	ra,40(sp)
   c:	02813023          	sd	s0,32(sp)
  10:	00913c23          	sd	s1,24(sp)
  14:	01213823          	sd	s2,16(sp)
  18:	01313423          	sd	s3,8(sp)
  1c:	00010993          	mv	s3,sp
  20:	00000917          	auipc	s2,0x0
  24:	3b890913          	addi	s2,s2,952 # 3d8 <printf+0x358>
  28:	fff00493          	li	s1,-1
  2c:	0ac00893          	li	a7,172
  30:	00000073          	ecall
  34:	00050413          	mv	s0,a0
  38:	00098613          	mv	a2,s3
  3c:	00040593          	mv	a1,s0
  40:	00090513          	mv	a0,s2
  44:	03c000ef          	jal	ra,80 <printf>
  48:	00048793          	mv	a5,s1
  4c:	fff7879b          	addiw	a5,a5,-1
  50:	fe079ee3          	bnez	a5,4c <main+0x48>
  54:	fd9ff06f          	j	2c <main+0x28>

Disassembly of section .text.putchar:

0000000000000058 <putchar>:
  58:	00000797          	auipc	a5,0x0
  5c:	7e078793          	addi	a5,a5,2016 # 838 <tail>
  60:	0007a703          	lw	a4,0(a5)
  64:	0017069b          	addiw	a3,a4,1
  68:	00d7a023          	sw	a3,0(a5)
  6c:	45000793          	li	a5,1104
  70:	00e787b3          	add	a5,a5,a4
  74:	00a78023          	sb	a0,0(a5)
  78:	00000513          	li	a0,0
  7c:	00008067          	ret

Disassembly of section .text.printf:

0000000000000080 <printf>:
  80:	f2010113          	addi	sp,sp,-224
  84:	08113c23          	sd	ra,152(sp)
  88:	08813823          	sd	s0,144(sp)
  8c:	08913423          	sd	s1,136(sp)
  90:	09213023          	sd	s2,128(sp)
  94:	07313c23          	sd	s3,120(sp)
  98:	07413823          	sd	s4,112(sp)
  9c:	07513423          	sd	s5,104(sp)
  a0:	07613023          	sd	s6,96(sp)
  a4:	05713c23          	sd	s7,88(sp)
  a8:	05813823          	sd	s8,80(sp)
  ac:	05913423          	sd	s9,72(sp)
  b0:	05a13023          	sd	s10,64(sp)
  b4:	03b13c23          	sd	s11,56(sp)
  b8:	00050413          	mv	s0,a0
  bc:	0ab13423          	sd	a1,168(sp)
  c0:	0ac13823          	sd	a2,176(sp)
  c4:	0ad13c23          	sd	a3,184(sp)
  c8:	0ce13023          	sd	a4,192(sp)
  cc:	0cf13423          	sd	a5,200(sp)
  d0:	0d013823          	sd	a6,208(sp)
  d4:	0d113c23          	sd	a7,216(sp)
  d8:	0a810913          	addi	s2,sp,168
  dc:	03213423          	sd	s2,40(sp)
  e0:	00000797          	auipc	a5,0x0
  e4:	7407ac23          	sw	zero,1880(a5) # 838 <tail>
  e8:	00054503          	lbu	a0,0(a0)
  ec:	26050663          	beqz	a0,358 <printf+0x2d8>
  f0:	00000a93          	li	s5,0
  f4:	00000493          	li	s1,0
  f8:	02500c93          	li	s9,37
  fc:	01500c13          	li	s8,21
 100:	3f400a13          	li	s4,1012
 104:	00810d13          	addi	s10,sp,8
 108:	00100993          	li	s3,1
 10c:	41a989bb          	subw	s3,s3,s10
 110:	fffd0d93          	addi	s11,s10,-1
 114:	2080006f          	j	31c <printf+0x29c>
 118:	00048a93          	mv	s5,s1
 11c:	1f40006f          	j	310 <printf+0x290>
 120:	020a8063          	beqz	s5,140 <printf+0xc0>
 124:	00093b83          	ld	s7,0(s2)
 128:	00890913          	addi	s2,s2,8
 12c:	00f00493          	li	s1,15
 130:	00249493          	slli	s1,s1,0x2
 134:	00900b13          	li	s6,9
 138:	ffc00a93          	li	s5,-4
 13c:	0200006f          	j	15c <printf+0xdc>
 140:	00092b83          	lw	s7,0(s2)
 144:	00890913          	addi	s2,s2,8
 148:	00700493          	li	s1,7
 14c:	fe5ff06f          	j	130 <printf+0xb0>
 150:	f09ff0ef          	jal	ra,58 <putchar>
 154:	ffc4849b          	addiw	s1,s1,-4
 158:	01548e63          	beq	s1,s5,174 <printf+0xf4>
 15c:	409bd7b3          	sra	a5,s7,s1
 160:	00f7f793          	andi	a5,a5,15
 164:	05778513          	addi	a0,a5,87
 168:	fefb44e3          	blt	s6,a5,150 <printf+0xd0>
 16c:	03078513          	addi	a0,a5,48
 170:	fe1ff06f          	j	150 <printf+0xd0>
 174:	00000a93          	li	s5,0
 178:	00000493          	li	s1,0
 17c:	1940006f          	j	310 <printf+0x290>
 180:	080a8463          	beqz	s5,208 <printf+0x188>
 184:	00093483          	ld	s1,0(s2)
 188:	00890913          	addi	s2,s2,8
 18c:	0804c463          	bltz	s1,214 <printf+0x194>
 190:	03000793          	li	a5,48
 194:	00f13423          	sd	a5,8(sp)
 198:	00013823          	sd	zero,16(sp)
 19c:	00013c23          	sd	zero,24(sp)
 1a0:	02010023          	sb	zero,32(sp)
 1a4:	04048c63          	beqz	s1,1fc <printf+0x17c>
 1a8:	000d0693          	mv	a3,s10
 1ac:	00000713          	li	a4,0
 1b0:	00a00613          	li	a2,10
 1b4:	02c4e7b3          	rem	a5,s1,a2
 1b8:	0307879b          	addiw	a5,a5,48
 1bc:	00f68023          	sb	a5,0(a3)
 1c0:	02c4c4b3          	div	s1,s1,a2
 1c4:	00070793          	mv	a5,a4
 1c8:	0017071b          	addiw	a4,a4,1
 1cc:	00168693          	addi	a3,a3,1
 1d0:	fe0492e3          	bnez	s1,1b4 <printf+0x134>
 1d4:	0207c463          	bltz	a5,1fc <printf+0x17c>
 1d8:	00fd04b3          	add	s1,s10,a5
 1dc:	00fd8ab3          	add	s5,s11,a5
 1e0:	02079793          	slli	a5,a5,0x20
 1e4:	0207d793          	srli	a5,a5,0x20
 1e8:	40fa8ab3          	sub	s5,s5,a5
 1ec:	0004c503          	lbu	a0,0(s1)
 1f0:	e69ff0ef          	jal	ra,58 <putchar>
 1f4:	fff48493          	addi	s1,s1,-1
 1f8:	fe9a9ae3          	bne	s5,s1,1ec <printf+0x16c>
 1fc:	00000a93          	li	s5,0
 200:	00000493          	li	s1,0
 204:	10c0006f          	j	310 <printf+0x290>
 208:	00092483          	lw	s1,0(s2)
 20c:	00890913          	addi	s2,s2,8
 210:	f7dff06f          	j	18c <printf+0x10c>
 214:	409004b3          	neg	s1,s1
 218:	02d00513          	li	a0,45
 21c:	e3dff0ef          	jal	ra,58 <putchar>
 220:	03000793          	li	a5,48
 224:	00f13423          	sd	a5,8(sp)
 228:	00013823          	sd	zero,16(sp)
 22c:	00013c23          	sd	zero,24(sp)
 230:	02010023          	sb	zero,32(sp)
 234:	f75ff06f          	j	1a8 <printf+0x128>
 238:	060a8863          	beqz	s5,2a8 <printf+0x228>
 23c:	00093783          	ld	a5,0(s2)
 240:	00890913          	addi	s2,s2,8
 244:	03000713          	li	a4,48
 248:	00e13423          	sd	a4,8(sp)
 24c:	00013823          	sd	zero,16(sp)
 250:	00013c23          	sd	zero,24(sp)
 254:	02010023          	sb	zero,32(sp)
 258:	04078e63          	beqz	a5,2b4 <printf+0x234>
 25c:	000d0713          	mv	a4,s10
 260:	00a00613          	li	a2,10
 264:	02c7e6b3          	rem	a3,a5,a2
 268:	0306869b          	addiw	a3,a3,48
 26c:	00d70023          	sb	a3,0(a4)
 270:	02c7c7b3          	div	a5,a5,a2
 274:	00e984bb          	addw	s1,s3,a4
 278:	00170713          	addi	a4,a4,1
 27c:	fe0794e3          	bnez	a5,264 <printf+0x1e4>
 280:	0004ce63          	bltz	s1,29c <printf+0x21c>
 284:	009d0ab3          	add	s5,s10,s1
 288:	000ac503          	lbu	a0,0(s5)
 28c:	dcdff0ef          	jal	ra,58 <putchar>
 290:	fff4849b          	addiw	s1,s1,-1
 294:	fffa8a93          	addi	s5,s5,-1
 298:	fe04d8e3          	bgez	s1,288 <printf+0x208>
 29c:	00000a93          	li	s5,0
 2a0:	00000493          	li	s1,0
 2a4:	06c0006f          	j	310 <printf+0x290>
 2a8:	00092783          	lw	a5,0(s2)
 2ac:	00890913          	addi	s2,s2,8
 2b0:	f95ff06f          	j	244 <printf+0x1c4>
 2b4:	00000493          	li	s1,0
 2b8:	fcdff06f          	j	284 <printf+0x204>
 2bc:	00890a93          	addi	s5,s2,8
 2c0:	00093483          	ld	s1,0(s2)
 2c4:	0004c503          	lbu	a0,0(s1)
 2c8:	06050c63          	beqz	a0,340 <printf+0x2c0>
 2cc:	d8dff0ef          	jal	ra,58 <putchar>
 2d0:	00148493          	addi	s1,s1,1
 2d4:	0004c503          	lbu	a0,0(s1)
 2d8:	fe051ae3          	bnez	a0,2cc <printf+0x24c>
 2dc:	000a8913          	mv	s2,s5
 2e0:	00000a93          	li	s5,0
 2e4:	00000493          	li	s1,0
 2e8:	0280006f          	j	310 <printf+0x290>
 2ec:	00890493          	addi	s1,s2,8
 2f0:	00094503          	lbu	a0,0(s2)
 2f4:	d65ff0ef          	jal	ra,58 <putchar>
 2f8:	00048913          	mv	s2,s1
 2fc:	00000a93          	li	s5,0
 300:	00000493          	li	s1,0
 304:	00c0006f          	j	310 <printf+0x290>
 308:	05950463          	beq	a0,s9,350 <printf+0x2d0>
 30c:	d4dff0ef          	jal	ra,58 <putchar>
 310:	00140413          	addi	s0,s0,1
 314:	00044503          	lbu	a0,0(s0)
 318:	04050063          	beqz	a0,358 <printf+0x2d8>
 31c:	fe0486e3          	beqz	s1,308 <printf+0x288>
 320:	f9d5051b          	addiw	a0,a0,-99
 324:	0ff57793          	andi	a5,a0,255
 328:	fefc64e3          	bltu	s8,a5,310 <printf+0x290>
 32c:	00279513          	slli	a0,a5,0x2
 330:	01450533          	add	a0,a0,s4
 334:	00052783          	lw	a5,0(a0)
 338:	014787b3          	add	a5,a5,s4
 33c:	00078067          	jr	a5
 340:	000a8913          	mv	s2,s5
 344:	00000a93          	li	s5,0
 348:	00000493          	li	s1,0
 34c:	fc5ff06f          	j	310 <printf+0x290>
 350:	00100493          	li	s1,1
 354:	fbdff06f          	j	310 <printf+0x290>
 358:	00000717          	auipc	a4,0x0
 35c:	4e070713          	addi	a4,a4,1248 # 838 <tail>
 360:	00072783          	lw	a5,0(a4)
 364:	0017869b          	addiw	a3,a5,1
 368:	00d72023          	sw	a3,0(a4)
 36c:	45000713          	li	a4,1104
 370:	00f707b3          	add	a5,a4,a5
 374:	00078023          	sb	zero,0(a5)
 378:	00000513          	li	a0,0
 37c:	00100793          	li	a5,1
 380:	04000893          	li	a7,64
 384:	00078513          	mv	a0,a5
 388:	00070593          	mv	a1,a4
 38c:	00068613          	mv	a2,a3
 390:	00000073          	ecall
 394:	00050513          	mv	a0,a0
 398:	0005051b          	sext.w	a0,a0
 39c:	09813083          	ld	ra,152(sp)
 3a0:	09013403          	ld	s0,144(sp)
 3a4:	08813483          	ld	s1,136(sp)
 3a8:	08013903          	ld	s2,128(sp)
 3ac:	07813983          	ld	s3,120(sp)
 3b0:	07013a03          	ld	s4,112(sp)
 3b4:	06813a83          	ld	s5,104(sp)
 3b8:	06013b03          	ld	s6,96(sp)
 3bc:	05813b83          	ld	s7,88(sp)
 3c0:	05013c03          	ld	s8,80(sp)
 3c4:	04813c83          	ld	s9,72(sp)
 3c8:	04013d03          	ld	s10,64(sp)
 3cc:	03813d83          	ld	s11,56(sp)
 3d0:	0e010113          	addi	sp,sp,224
 3d4:	00008067          	ret
