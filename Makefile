program_name = cdev

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc test_cdev.c -o test_cdev
	gcc test_debugfs.c -o test_debugfs
	gcc test_ioctl.c -o test_ioctl 
	gcc test_joystick.c -o test_joystick
	gcc test_netlink.c -o test_netlink

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
