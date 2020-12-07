# Makefile_kernel
DEP = ../include/sched.h ${TOP}/Makefile

.PHONY: all
all: head.o entry.o strap.o sched.o vm.o
%.o: %.S ${DEP}
	@${GCC} ${CFLAG} -c $< -o $@

%.o: %.c ${DEP}
	@${GCC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm -f *.o
