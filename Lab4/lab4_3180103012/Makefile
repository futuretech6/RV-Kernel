# Makefile_lab4
# ln -s ~/lab4/.gdbinit ~/.gdbinit
# sed -i '$aalias md=make\ &&\ make\ debug' ~/.bashrc
# sed -i '$aalias gdb=/opt/riscv/bin/riscv64-unknown-linux-gnu-gdb' ~/.bashrc
# sed -i '$aalias gv=/opt/riscv/bin/riscv64-unknown-linux-gnu-gdb\ vmlinux' ~/.bashrc
# sed -i '$aalias objdump=/opt/riscv/bin/riscv64-unknown-linux-gnu-objdump' ~/.bashrc
# sed -i '$aalias ov=/opt/riscv/bin/riscv64-unknown-linux-gnu-objdump\ -S\ vmlinux' ~/.bashrc
export TOP=$(shell pwd)
export RISCV=/opt/riscv
export PATH:=${PATH}:${RISCV}/bin

export
CROSS_=riscv64-unknown-elf-
AR=${CROSS_}ar
CC=${CROSS_}gcc
LD=${CROSS_}ld
OBJCOPY=${CROSS_}objcopy
OBJDUMP=${CROSS_}objdump

ISA ?= rv64imafd
ABI ?= lp64

INCLUDE = -I ${TOP}/include/ -I ${TOP}/arch/riscv/include/
CF = -O0 -march=$(ISA) -mabi=$(ABI) -mcmodel=medany -ffunction-sections -fdata-sections -nostartfiles -nostdlib -nostdinc -static -lgcc -Wl,--nmagic -Wl,--gc-sections
# CFLAG = ${CF} ${INCLUDE} -g -D SJF
# CFLAG = ${CF} ${INCLUDE} -g -D PRIORITY
CFLAG = ${CF} ${INCLUDE} -g

.PHONY: all
all:
	@make -C ${TOP}/lib/
	@make -C ${TOP}/init/
	@make -C ${TOP}/arch/riscv/kernel/
	@make -C ${TOP}/arch/riscv/
	@echo "\e[35m==== Build Successfully ====\e[0m"
	${OBJDUMP} -S ${TOP}/vmlinux > ${TOP}/dasm.S
	@echo "\e[35m==== DeAsm Successfully ====\e[0m"

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
	@rm -f ${TOP}/vmlinux ${TOP}/System.map ${TOP}/dasm.S
	@make -C ${TOP}/init/ clean
	@make -C ${TOP}/lib/ clean
	@make -C ${TOP}/arch/riscv/kernel/ clean
	@make -C ${TOP}/arch/riscv/ clean
	@echo "\e[36m==== Clean Successfully ====\e[0m"
