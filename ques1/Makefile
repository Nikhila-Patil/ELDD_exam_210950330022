        obj-m := ioctl.o
        KERNELDIR = /lib/modules/$(shell uname -r)/build
        PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -o usrap1 usrap1.c

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -rf application

