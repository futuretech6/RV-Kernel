# Makefile_lib
DEP = ./Makefile $(wildcard ${TOP}/include/*.h)
SRC = $(wildcard *.c)

.PHONY: all clean


all: $(SRC:%.c=%.o)
%.o: %.c ${DEP}
	@${CC} ${CFLAG} -c $< -o $@


clean:
	@rm *.o 
