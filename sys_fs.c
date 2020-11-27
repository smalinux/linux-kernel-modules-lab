/*
 * file       sys_fs.c
 * details    Simple Linux device driver (sysfs)
 * author     smalinux
 *
 * What's sys fs as a system admin?
 * please see Documentation/filesystems/sysfs.rst
 *
 * How to use:
 * $ insmod sys_fs.ko
 * $ ls /sys/kernel/sma_sysfs/
 * $ cat /sys/kernel/sma_sysfs/sma_value
 * $ echo 44 > /sys/kernel/sma_sysfs/sma_value
 * $ cat /sys/kernel/sma_sysfs/sma_value
 * $ rmmod sys_fs
 *
 */ 
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/sysfs.h> 
#include <linux/kobject.h> 
 
 
volatile int sma_value = 0;
 
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev sma_cdev;
struct kobject *kobj_ref;

/*
** Function Prototypes
*/
static int      __init sma_driver_init(void);
static void     __exit sma_driver_exit(void);
 
/*************** Driver Fuctions **********************/
static int      sma_open(struct inode *inode, struct file *file);
static int      sma_release(struct inode *inode, struct file *file);
static ssize_t  sma_read(struct file *filp, 
                        char __user *buf, size_t len,loff_t * off);
static ssize_t  sma_write(struct file *filp, 
                        const char *buf, size_t len, loff_t * off);
 
/*************** Sysfs Fuctions **********************/
static ssize_t  sysfs_show(struct kobject *kobj, 
                        struct kobj_attribute *attr, char *buf);
static ssize_t  sysfs_store(struct kobject *kobj, 
                        struct kobj_attribute *attr,const char *buf, size_t count);

struct kobj_attribute sma_attr = __ATTR(sma_value, 0660, sysfs_show, sysfs_store);

/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = sma_read,
        .write          = sma_write,
        .open           = sma_open,
        .release        = sma_release,
};

/*
** This fuction will be called when we read the sysfs file
*/
static ssize_t sysfs_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf)
{
        printk(KERN_INFO "Sysfs - Read!!!\n");
        return sprintf(buf, "%d", sma_value);
}

/*
** This fuction will be called when we write the sysfsfs file
*/
static ssize_t sysfs_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count)
{
        printk(KERN_INFO "Sysfs - Write!!!\n");
        sscanf(buf,"%d",&sma_value);
        return count;
}

/*
** This fuction will be called when we open the Device file
*/ 
static int sma_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

/*
** This fuction will be called when we close the Device file
*/ 
static int sma_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}
 
/*
** This fuction will be called when we read the Device file
*/
static ssize_t sma_read(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read function\n");
        return 0;
}

/*
** This fuction will be called when we write the Device file
*/
static ssize_t sma_write(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write Function\n");
        return 0;
}
 
/*
** Module Init function
*/
static int __init sma_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "sma_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&sma_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&sma_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"sma_class")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"sma_device")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
 
        /*Creating a directory in /sys/kernel/ */
        kobj_ref = kobject_create_and_add("sma_sysfs", kernel_kobj);
	if(!kobj_ref)
		return -ENOMEM;
 
        /*Creating sysfs file for sma_value*/
        if(sysfs_create_file(kobj_ref, &sma_attr.attr)){
                printk(KERN_INFO"Cannot create sysfs file......\n");
                goto r_sysfs;
    }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
        return 0;
 
r_sysfs:
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &sma_attr.attr);
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&sma_cdev);
        return -1;
}

/*
** Module exit function
*/
static void __exit sma_driver_exit(void)
{
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &sma_attr.attr);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&sma_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(sma_driver_init);
module_exit(sma_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("smalinux <xunilams@gmail.com>");
MODULE_DESCRIPTION("Simple Linux device driver (sysfs)");
MODULE_VERSION("1.3");
