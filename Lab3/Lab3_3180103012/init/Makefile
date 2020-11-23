# Makefile_init
DEP = ${TOP}/include/put.h ${TOP}/include/test.h ${TOP}/arch/riscv/include/sched.h

.PHONY: all
all: main.o test.o

%.o: %.c ${DEP}
	@${GCC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm *.o 
