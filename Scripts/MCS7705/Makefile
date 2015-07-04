#########################################################
#             Makefile for MOS7705   LINUX 2.6          #      
#########################################################

KDIR:=/lib/modules/$(shell uname -r)/build

EXTRA_CFLAGS += -I $(KDIR)/drivers/usb/serial

obj-m:=mos7705.o

default:
	$(RM) *.mod.c *.o *.ko .*.cmd *.symvers ioctl
	$(MAKE) -C $(KDIR) $(EXTRA_CFLAGS) SUBDIRS=$(PWD) modules
	gcc -pthread ioctl.c -o ioctl

clean:
	rm -rf *.mod.c *.o *.ko .*.cmd *.symvers .tmp_versions *.order ioctl *~

load: 
	modprobe usbserial
	insmod mos7705.ko
	modprobe lp

unload:
	modprobe -r lp
	rmmod mos7705

install:
	sh mosinstall

uninstall:
	sh mosuninstall







