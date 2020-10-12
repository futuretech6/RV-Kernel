# Makefile_init
.PHONY: all
all: main.o test.o

%.o: %.c
	@${GCC} ${CFLAG} -c $< -o $@

.PHONY: clean
clean:
	@rm *.o 
