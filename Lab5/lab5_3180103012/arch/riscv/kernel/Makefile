# Makefile_kernel
DEP = ./Makefile ${TOP}/Makefile $(shell ls ${TOP}/include/*.h) $(shell ls ${TOP}/arch/riscv/include/*.h)
SRC = $(shell ls *.c *.S)

.PHONY: all clean c asm


all: c asm
c: $(SRC:%.c=%.o) ${DEP}
%.o: %.S ${DEP} ${SRC}
	@${CC} $(CFLAG) -c $< -o $@

asm: $(SRC:%.S=%.o) ${DEP}
%.o: %.c ${DEP}
	@${CC} ${CFLAG} -c $< -o $@


clean:
	@rm -f *.o
