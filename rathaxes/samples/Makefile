obj-m += rtx_rs232.o
rtx_rs232-objs := serial_normalise.o

DEVICE = my_rs232


all: linux

linux:
		rathaxes linux serial_normalise.rtx
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

windows:
		rathaxes windows serial_normalise.rtx

clean:
		rathaxes clean
		rm -rf rathaxesHeaderTemp
		rm -rf *.o
		rm -rf *.mod.c
		rm -rf Module.symvers
		rm -rf .serial_normalise.o.d
		rm -rf .tmp_versions
		rm -rf .rtx_rs232.ko.cmd
		rm -rf .rtx_rs232.mod.o.cmd
		rm -rf .rtx_rs232.o.cmd
		rm -rf .serial_normalise.o.cmd
		rm -rf modules.order

fclean: clean
		rm -f *.ko

re: fclean all

load:
	insmod rtx_rs232.ko 
	sed -n "s/[^0-9]*\([0-9]*\) $(DEVICE)/mknod \/dev\/$(DEVICE) c \1 0/p" /proc/devices | sh

unload:
	sed -n "s/[^0-9]*\([0-9]*\) $(DEVICE)/rm \/dev\/$(DEVICE)" /proc/devices | sh
	rmmod rtx_rs232
