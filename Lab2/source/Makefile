# Makefile_lab2
# alias gdb=/opt/riscv/bin/riscv64-unknown-linux-gnu-gdb
export TOP=$(shell pwd)
export RISCV=/opt/riscv
export PATH:=${PATH}:${RISCV}/bin

export
CROSS_=riscv64-unknown-elf-
AR=${CROSS_}ar
GCC=${CROSS_}gcc
LD=${CROSS_}ld
OBJCOPY=${CROSS_}objcopy

ISA ?= rv64imafd
ABI ?= lp64

INCLUDE = -I ../include
CF = -O0 -march=$(ISA) -mabi=$(ABI) -mcmodel=medany -ffunction-sections -fdata-sections -nostartfiles -nostdlib -nostdinc -static -lgcc -Wl,--nmagic -Wl,--gc-sections
CFLAG = ${CF} ${INCLUDE} -g

.PHONY: all
all:
	@make -C ./lib/
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
	@echo "\e[32m==== Start Debugging ====\e[0m"
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux -S -s
	@echo "\e[32m==== End Debugging ====\e[0m"

.PHONY: clean
clean:
	@rm -f ./vmlinux ./System.map
	@make -C ./init/ clean
	@make -C ./lib/ clean
	@make -C ./arch/riscv/kernel/ clean
	@make -C ./arch/riscv/ clean
	@echo "\e[36m==== Clean Successfully ====\e[0m"
