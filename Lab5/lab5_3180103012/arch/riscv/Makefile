# Makefile_riscv
OBJ = $(shell ls ${TOP}/init/*.o) $(shell ls ${TOP}/lib/*.o) $(shell ls ${TOP}/arch/riscv/kernel/*.o)

.PHONY: all clean


all: ./Makefile ${TOP}/vmlinux ./boot/Image ${TOP}/System.map
${TOP}/vmlinux: ${OBJ} ./kernel/vmlinux.lds ./Makefile
	@# link to get the vmlinux
	@${LD} ${OBJ} -T ./kernel/vmlinux.lds -o ${TOP}/vmlinux

./boot/Image: ${TOP}/vmlinux
	@# use vmlinux to gen Image
	@${OBJCOPY} ${TOP}/vmlinux ./boot/Image

${TOP}/System.map: ${TOP}/vmlinux
	@# print kernel symbol table
	@nm ${TOP}/vmlinux > ${TOP}/System.map


clean:
	@rm -f ${TOP}/vmlinux ./boot/Image ${TOP}/System.map