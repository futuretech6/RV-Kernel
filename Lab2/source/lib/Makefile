# Makefile_lib
.PHONY: all
all: put.o

%.o: %.c
	@${GCC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm *.o 
