obj-m += info_sistema.o
obj-m += io_input_gutierrez_zerpa.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(pwd) clean



 
