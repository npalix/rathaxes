obj-m += rtx_rs232.o
rtx_rs232-objs := rs232_mod.o rs232.o


all:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules


clean:
		rm -f	*.o
		rm -f	*.mod.c
		rm -f	Module.symvers

fclean: clean
		rm -f *.ko

re: fclean all

