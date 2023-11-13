all: build

build: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)/module modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)/module clean

install:
	sudo cp kpubsub.ko /usr/lib/modules