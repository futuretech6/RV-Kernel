# Makefile_lab1
# export TOP=~/lab1/
# export RISCV=/opt/riscv
# export PATH=$PATH:$RISCV/bin

export
CROSS_= riscv64-unknown-elf-
AR=${CROSS_}ar
GCC=${CROSS_}gcc
LD=${CROSS_}ld
OBJCOPY=${CROSS_}objcopy

ISA ?= rv64imafd
ABI ?= lp64

INCLUDE = -I ../include
CF = -O3 -march=$(ISA) -mabi=$(ABI) -mcmodel=medany -ffunction-sections -fdata-sections -nostartfiles -nostdlib -nostdinc -static -lgcc -Wl,--nmagic -Wl,--gc-sections
CFLAG = ${CF} ${INCLUDE}

.PHONY: all
all:
	@make -C ./init/
	@make -C ./arch/riscv/kernel/
	@make -C ./arch/riscv/
	@echo "\e[35m==== Build Successfully ====\e[0m"

.PHONY: run
run:
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux
	@echo "\e[32m==== Run Successfully ====\e[0m"

.PHONY: debug
debug:
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux -S -s
	# riscv64-unknown-linux-gnu-gdb vmlinux

.PHONY: clean
clean:
	@rm -f ./vmlinux ./System.map
	@make -C ./init/ clean
	@make -C ./arch/riscv/kernel/ clean
	@make -C ./arch/riscv/ clean
	@echo "\e[36m==== Clean Successfully ====\e[0m"
