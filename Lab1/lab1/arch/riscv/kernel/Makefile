# Makefile_kernel
.PHONY: all
all:head.o

head.o: head.S
	@${GCC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm -f *.o
