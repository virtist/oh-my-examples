MODULE_NAME		:= hello
PWD				:= $(shell pwd)
EXTRA_CFLAGS	:= -Wall -DKERNEL

STRIP		?= $(CROSS_COMPILE)strip
OBJCOPY		?= $(CROSS_COMPILE)objcopy

KERNEL_SRC	?= /lib/modules/$(shell uname -r)/build
ifneq ($(wildcard $(KERNEL_SRC).*),)
	KERNEL_DIR  ?= $(KERNEL_SRC)
else
    KERNEL_DIR  ?= ~/linux
endif

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

all: hello

hello:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	
strip:
	@echo "Generate debuginfo & strip module..."
	@$(OBJCOPY) --only-keep-debug $(MODULE_NAME).ko $(MODULE_NAME).debug
	@$(STRIP) --strip-debug $(MODULE_NAME).ko
	@$(OBJCOPY) --add-gnu-debuglink=$(MODULE_NAME).debug $(MODULE_NAME).ko

format:
	@echo "Format source files..."
	@find . -name "*.h" -o -name "*.c" | xargs clang-format -i

test: hello
	sudo insmod (MODULE_NAME).ko run_ktest=y
	sudo rmmod (MODULE_NAME)
	sudo dmesg | grep '\[ktest\]'

qemu: hello
	gcc -static qemu_init.c -o init
	find init $(MODULE_NAME).ko | cpio -o -H newc | gzip -9c > initrd.gz
	qemu-system-x86_64 -nodefaults -m 256M -kernel $(KERNEL_DIR)/arch/x86/boot/bzImage \
		-append "console=ttyS0" \
		-initrd initrd.gz \
		-no-reboot -nographic -serial stdio

clean:
	rm -f init
	find ./ -regextype posix-extended -regex ".*\.(ko|o|mod.c|order|symvers|d|cmd|mod|o.tmp|debug)" | xargs rm -f
