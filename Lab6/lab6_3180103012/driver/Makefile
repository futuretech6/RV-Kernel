ASM_SRC		= $(sort $(wildcard *.S))
C_SRC       = $(sort $(wildcard *.c))
OBJ		    = $(patsubst %.S,%.o,$(ASM_SRC)) $(patsubst %.c,%.o,$(C_SRC))

.PHONY: all clean


all: $(OBJ)
%.o:%.c
	@${CC} ${CFLAG} -c $< -o $@


clean:
	@rm -f *.o
