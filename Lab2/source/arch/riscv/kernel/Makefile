# Makefile_kernel
.PHONY: all
all: head.o entry.o strap.o

%.o: %.S
	@${GCC} ${CFLAG} -c $< -o $@

strap.o: strap.c
	@${GCC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm -f *.o
