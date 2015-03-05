EXTRA_CFLAGS +=  -Werror -Wno-declaration-after-statement
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
KMAKE := make -C $(KERNEL_DIR) M=$(PWD)

obj-m += MyMonitor.o

all:
	$(KMAKE) modules

clean:
	$(KMAKE) clean
