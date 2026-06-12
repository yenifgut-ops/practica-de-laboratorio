obj-m += info_sistema.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(pwd) clean

obj-m += io_input_gutierrez_zerpa.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
