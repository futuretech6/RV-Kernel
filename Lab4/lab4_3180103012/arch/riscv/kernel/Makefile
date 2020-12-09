# Makefile_kernel
DEP = ./Makefile ../include/sched.h ../include/vm.h ${TOP}/Makefile

.PHONY: all
all: head.o entry.o strap.o sched.o vm.o ${DEP}
%.o: %.S ${DEP}
	@${CC} ${CFLAG} -c $< -o $@

%.o: %.c ${DEP}
	@${CC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm -f *.o
