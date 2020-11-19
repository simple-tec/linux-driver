ifneq ($(KERNELRELEASE),)
obj-m := charDrv.o
else
PWD := $(shell pwd)
#KDIR:= /root/kernel/linux-2.6.32.2
KDIR := /lib/modules/`uname -r`/build
all:
	make -C $(KDIR) M=$(PWD)
clean:	
	rm -rf *.o *.ko *.mod.c *.symvers *.c~ *~
endif
