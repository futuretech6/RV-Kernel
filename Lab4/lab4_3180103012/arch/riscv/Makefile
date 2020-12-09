# Makefile_riscv
.PHONY: all
all: ./Makefile ${TOP}/vmlinux ./boot/Image ${TOP}/System.map

OBJS = ${TOP}/init/main.o ${TOP}/init/test.o ${TOP}/lib/put.o ${TOP}/lib/rand.o ./kernel/head.o ./kernel/entry.o ./kernel/strap.o ./kernel/sched.o ./kernel/vm.o

${TOP}/vmlinux: ${OBJS} ./kernel/vmlinux.lds ./Makefile
	@# link to get the vmlinux
	@${LD} ${OBJS} -T ./kernel/vmlinux.lds -o ${TOP}/vmlinux

./boot/Image: ${TOP}/vmlinux
	@# use vmlinux to gen Image
	@${OBJCOPY} ${TOP}/vmlinux ./boot/Image

${TOP}/System.map: ${TOP}/vmlinux
	@# print kernel symbol table
	@nm ${TOP}/vmlinux > ${TOP}/System.map

.PHONY: clean
clean:
	@rm -f ${TOP}/vmlinux ./boot/Image ${TOP}/System.map