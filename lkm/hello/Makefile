MODULE_NAME		:= hello
PWD				:= $(shell pwd)
EXTRA_CFLAGS	:= -Wall -DKERNEL

STRIP		?= $(CROSS_COMPILE)strip
OBJCOPY		?= $(CROSS_COMPILE)objcopy

KERNEL_DIR	?= /lib/modules/$(shell uname -r)/build
$(info KERNEL_DIR = $(KERNEL_DIR))

#
# source files
#
obj-m 				:= $(MODULE_NAME).o
$(MODULE_NAME)-objs	:= \
	main.o \
	ktest.o \
	ktest_example.o \
	bowling_game.o

all: mod
.PHONY: strip format test qemu clean

mod:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	
strip:
	@echo "Generate debuginfo & strip module..."
	@$(OBJCOPY) --only-keep-debug $(MODULE_NAME).ko $(MODULE_NAME).debug
	@$(STRIP) --strip-debug $(MODULE_NAME).ko
	@$(OBJCOPY) --add-gnu-debuglink=$(MODULE_NAME).debug $(MODULE_NAME).ko

format:
	@echo "Format source files..."
	@find . -name "*.h" -o -name "*.c" | xargs clang-format -i

test: mod
	dmesg -c > /dev/null
	sudo insmod $(MODULE_NAME).ko run_ktest=y
	sudo rmmod $(MODULE_NAME)
	sudo dmesg | grep '\[ktest\]'

init: qemu_init.c
	gcc -static -DMODNAME=\"/$(MODULE_NAME).ko\" -DMODPARAM=\"run_ktest=y\" qemu_init.c -o init

initrd.gz: init mod
	find init $(MODULE_NAME).ko | cpio -o -H newc | gzip -9c > initrd.gz

qemu: initrd.gz
	qemu-system-x86_64 -nodefaults -m 256M -kernel $(KERNEL_DIR)/arch/x86/boot/bzImage \
		-append "console=ttyS0" \
		-initrd initrd.gz \
		-no-reboot -nographic -serial stdio

clean:
	rm -f init initrd.gz
	find ./ -regextype posix-extended -regex ".*\.(ko|o|mod.c|order|symvers|d|cmd|mod|o.tmp|debug)" | xargs rm -f
