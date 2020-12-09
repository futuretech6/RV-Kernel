# Makefile_init
DEP = ./Makefile ${TOP}/include/test.h ${TOP}/include/put.h ${TOP}/arch/riscv/include/sched.h

.PHONY: all
all: main.o test.o

%.o: %.c ${DEP}
	@${CC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm *.o 
