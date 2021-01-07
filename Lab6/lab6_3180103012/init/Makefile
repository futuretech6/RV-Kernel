# Makefile_init
DEP = ./Makefile $(wildcard ${TOP}/include/*.h) $(wildcard ${TOP}/arch/riscv/include/*.h)
SRC = $(wildcard *.c)

.PHONY: all clean


all: $(patsubst %c, %o, ${SRC})
%.o: %.c ${DEP}
	@${CC} ${CFLAG} -c $< -o $@


clean:
	@rm -f *.o 
