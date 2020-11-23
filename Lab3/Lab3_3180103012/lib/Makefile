# Makefile_lib
DEP = ${TOP}/include/put.h ${TOP}/include/rand.h

.PHONY: all
all: put.o rand.o

%.o: %.c ${DEP}
	@${GCC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm *.o 
