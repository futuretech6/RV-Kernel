# Makefile_lib
DEP = ./Makefile ${TOP}/include/put.h ${TOP}/include/rand.h

.PHONY: all
all: put.o rand.o

%.o: %.c ${DEP}
	@${CC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm *.o 
