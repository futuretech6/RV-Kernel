# Makefile_kernel
DEP = ./Makefile ${TOP}/Makefile $(wildcard ${TOP}/include/*.h) $(wildcard ${TOP}/arch/riscv/include/*.h)
SRC = $(wildcard *.c *.S)

.PHONY: all clean c asm


all: c asm
c: $(patsubst %c, %o, ${SRC}) ${DEP}
%.o: %.S ${DEP}
	@${CC} $(CFLAG) -c $< -o $@


asm: $(patsubst %S, %o, ${SRC}) ${DEP}
%.o: %.c ${DEP}
	@${CC} ${CFLAG} -c $< -o $@


clean:
	@rm -f *.o
