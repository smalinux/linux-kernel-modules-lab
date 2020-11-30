#!/bin/sh

#load module
insmod myfs.ko

#mount filesystem
mkdir -p /mnt/myfs
mount -t myfs none /mnt/myfs
ls -laid /mnt/myfs

#show registered filesystems
cat /proc/filesystems | grep myfs

#show mounted filesystems
cat /proc/mounts | grep myfs

#show filesystem statistics
stat -f /mnt/myfs

cd /mnt/myfs

# create directory
mkdir mydir
ls -la

# create subdirectory
cd mydir
mkdir mysubdir
ls -lai

# rename subdirectory
mv mysubdir myrenamedsubdir
ls -lai

# delete renamed subdirectory
rmdir myrenamedsubdir
ls -la

# create file
touch myfile
ls -lai

# rename file
mv myfile myrenamedfile
ls -lai

# delete renamed file
rm myrenamedfile

# delete directory
cd ..
rmdir mydir
ls -la

# create file
touch myfile
ls -lai

# rename file
mv myfile myrenamedfile
ls -lai

# create link to file
ln myrenamedfile mylink
ls -lai

# read/write file
echo "message" > myrenamedfile
cat myrenamedfile

# remove link to file
rm mylink
ls -la

# delete file
rm -f myrenamedfile
ls -la

#list all filesystem files
cd /mnt/myfs
ls -la

#unmount filesystem
cd ..
umount /mnt/myfs

#unload module
rmmod myfs
