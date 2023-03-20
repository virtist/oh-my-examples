MODULE_NAME		:= hello
PWD				:= $(shell pwd)
EXTRA_CFLAGS	:= -Wall -DKERNEL

KERNEL_DIR	?= /lib/modules/$(shell uname -r)/build
STRIP		?= $(CROSS_COMPILE)strip
OBJCOPY		?= $(CROSS_COMPILE)objcopy

#
# source files
#
obj-m 				:= $(MODULE_NAME).o
$(MODULE_NAME)-objs	:= \
	main.o \
	foo.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	
strip:
	@echo "Generate debuginfo & strip module..."
	@$(OBJCOPY) --only-keep-debug $(MODULE_NAME).ko $(MODULE_NAME).debug
	@$(STRIP) --strip-debug $(MODULE_NAME).ko
	@$(OBJCOPY) --add-gnu-debuglink=$(MODULE_NAME).debug $(MODULE_NAME).ko

format:
	@echo "Format source files..."
	@find . -name "*.h" -o -name "*.c" | xargs clang-format -i

clean:
	find ./ -regextype posix-extended -regex ".*\.(ko|o|mod.c|order|symvers|d|cmd|mod|o.tmp|debug)" | xargs rm -f
