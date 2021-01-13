# Makefile_lab6
# ln -s ~/lab6/.gdbinit ~/.gdbinit
# sed -i '$aalias md=make\\ debug' ~/.bashrc
# sed -i '$aalias gdb=/opt/riscv/bin/riscv64-unknown-linux-gnu-gdb' ~/.bashrc
# sed -i '$aalias gv=/opt/riscv/bin/riscv64-unknown-linux-gnu-gdb\\ vmlinux' ~/.bashrc
# sed -i '$aalias readelf=/opt/riscv/bin/riscv64-unknown-linux-gnu-readelf' ~/.bashrc
# sed -i '$aalias objdump=/opt/riscv/bin/riscv64-unknown-linux-gnu-objdump' ~/.bashrc
# /opt/riscv/bin/riscv64-unknown-linux-gnu-readelf
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
READELF=${CROSS_}readelf

ISA ?= rv64imafd
ABI ?= lp64

INCLUDE = -I ${TOP}/include/ -I ${TOP}/arch/riscv/include/
CF = -march=$(ISA) -mabi=$(ABI) -mcmodel=medany -ffunction-sections -fdata-sections -nostartfiles -nostdlib -nostdinc -static -lgcc -Wl,--nmagic -Wl,--gc-sections -g -w
# CFLAG = ${CF} ${INCLUDE} -D SJF
CFLAG = ${CF} ${INCLUDE} -D PRIORITY

.PHONY: all user kernel dasm run debug clean


all: user kernel dasm


user:
	@make -C ${TOP}/user/
	@echo "\e[35mBuild User Program Successfully\e[0m"


kernel:
	@make -C ${TOP}/init/
	@make -C ${TOP}/driver/
	@make -C ${TOP}/lib/
	@make -C ${TOP}/arch/riscv/kernel/
	@make -C ${TOP}/arch/riscv/
	@echo "\e[35mBuild Kernel Successfully\e[0m"


dasm: ${TOP}/misc/dasm.S ${TOP}/misc/vmlinux_section.txt ${TOP}/misc/hello_dasm.S ${TOP}/misc/hello_section.txt
	@echo "\e[35mDisassemble Successfully\e[0m"
${TOP}/misc/dasm.S: ${TOP}/vmlinux
	${OBJDUMP} -S ${TOP}/vmlinux > ${TOP}/misc/vmlinux_dasm.S
${TOP}/misc/vmlinux_section.txt: ${TOP}/vmlinux
	${READELF} -S ${TOP}/vmlinux > ${TOP}/misc/vmlinux_section.txt
${TOP}/misc/hello_dasm.S: ${TOP}/user/hello.elf
	${OBJDUMP} -S ${TOP}/user/hello.elf > ${TOP}/misc/hello_dasm.S
${TOP}/misc/hello_section.txt: ${TOP}/user/hello.elf
	${READELF} -S ${TOP}/user/hello.elf > ${TOP}/misc/hello_section.txt


run: user kernel dasm
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux -initrd ./user/hello.bin
	@echo "\e[32mRun Successfully\e[0m"


debug: user kernel dasm
	@echo "\e[32mStart Debugging\e[0m"
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux -initrd ./user/hello.bin -S -s
	@echo "\e[32mEnd Debugging\e[0m"


clean:
	@rm -f ${TOP}/vmlinux ${TOP}/System.map ${TOP}/a.out
	@make -C ${TOP}/arch/riscv/ clean
	@make -C ${TOP}/arch/riscv/kernel/ clean
	@make -C ${TOP}/init/ clean
	@make -C ${TOP}/lib/ clean
	@make -C ${TOP}/driver/ clean
	@make -C ${TOP}/user/ clean
	@echo "\e[36mClean Successfully\e[0m"
