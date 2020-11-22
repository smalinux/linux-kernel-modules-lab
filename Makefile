obj-m += hello.o			# hello world module
#obj-m += oops.o			# make oops ...
obj-m += invoke-userlevel-app-2.o 	# invoke /sbin/shutdown
obj-m += joystick.o
#obj-m += net.o
#obj-m += netfilter.o
obj-m += list.o				# <linux/list.h>
obj-m += per-cpu.o			# per-cpu variable
obj-m += hashtable.o			# <linux/hashtable.h>
obj-m += phone_table.o			# simple hashtable.h example
obj-m += container_of.o			# understanding container_macro ..
obj-m += container_of_container_of.o
obj-m += string-9.o			# string manipulation
obj-m += kfifo_int.o			# /samples/kfifo/inttype-example.c
obj-m += wait_queue.o			#
obj-m += wait_queue_proc.o		#
obj-m += waitqueue2.o			# full static example
obj-m += waitqueue3.o			# dynamic version
obj-m += msleep.o			#
obj-m += params.o			# argc & argv in kernel level
obj-m += cb_params.o			# callback when var change
obj-m += proc_fs.o			# proc fs full example (V1)
obj-m += proc_fs2.o			# char dev with proc_fs
obj-m += sys_fs.o			# sys filesystem
obj-m += cdev.o 			# complete char device
obj-m += ioctl.o 			# ioctl
obj-m += work_queue.o			# -------
obj-m += list2.o			# -------
#obj-m += input.o 			# testing linux input subsystem




program_name = cdev

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc test_cdev.c -o test_cdev
	gcc test_ioctl.c -o test_ioctl 
	gcc test_joystick.c -o test_joystick

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
