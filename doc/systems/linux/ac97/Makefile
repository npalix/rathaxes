KDIR =	/lib/modules/$(shell uname -r)/build
obj-m := rtx_ac97.o

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install
	depmod -a
clean:
	rm -rf *~
	$(MAKE) -C $(KDIR) M=$(PWD) clean
