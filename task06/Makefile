
obj-m += hello-world.o
CFLAGS_hello-world.o := -DDEBUG

all:
	make -C $(KERNEL_PATH) M=$(PWD) modules

clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
