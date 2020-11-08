obj-m += per-cpu-1.o
obj-m += hello.o			# hello world module
obj-m += oops.o				# make oops ...
obj-m += invoke-userlevel-app-2.o 	# invoke /sbin/shutdown
obj-m += joystick.o
obj-m += net.o
#obj-m += netfilter.o
obj-m += list.o				# <linux/list.h>
obj-m += hashtable.o			# <linux/hashtable.h>
obj-m += container_of.o			# Now I understand container_macro deeply ..
obj-m += container_of_container_of.o
#obj-m += cdev.o 			# complete char device


program_name = cdev

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

# Utilities for fast testing ...
i:
	insmod $(program_name).ko	# insert $ make i
r:
	rmmod $(program_name)		# remove $ make r
p:
	dmesg				# print  $ make p
c:
	dmesg -c		# clear dmesg	 $ make c
info:
	modinfo $(program_name).ko #userspace program prints .modinfo section
