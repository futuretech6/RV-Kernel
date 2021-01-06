# This file is part of Zhejiang University Operating System Courses Project
# This program is under MIT license, see http://phvntom.tech/LICENSE.txt

CROSS_PREFIX	=	riscv64-unknown-elf-
CC				=	$(CROSS_PREFIX)gcc
LD				=	$(CROSS_PREFIX)ld
AR				= 	$(CROSS_PREFIX)ar
OBJCOPY			=	$(CROSS_PREFIX)objcopy

ISA			=	rv64ima
ABI			=	lp64
CFLAG		=	-march=$(ISA) -mabi=$(ABI) -mcmodel=medany \
			-ffunction-sections -fdata-sections -nostartfiles \
			-nostdlib -nostdinc -fno-builtin \
			-g3 -static -lgcc -O1 -Wl,--gc-section -Wl,--print-gc-sections

ASM_SRC	    = $(sort $(wildcard *.S))
C_SRC       = $(sort $(wildcard *.c))
OBJ	    = $(patsubst %.S,%.o,$(ASM_SRC)) $(patsubst %.c,%.o,$(C_SRC))

all: hello.bin

%.o:%.c
	$(CC) $(CFLAG) -c $<

%.o:%.S
	$(CC) $(CFLAG)  -c $<

hello.bin: $(OBJ)
	$(CC) $(CFLAG) -T link.ld -o hello.elf $(OBJ)
	$(OBJCOPY) hello.elf -O binary hello.bin

clean:
	rm -rf *.o hello.*
