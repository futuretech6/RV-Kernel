# Makefile_lib
DEP = ./Makefile $(shell ls ${TOP}/include/*.h)
SRC = $(shell ls *.c)

.PHONY: all clean


all: $(SRC:%.c=%.o)
%.o: %.c ${DEP}
	@${CC} ${CFLAG} -c $< -o $@


clean:
	@rm *.o 
