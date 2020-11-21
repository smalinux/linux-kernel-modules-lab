/*
 * @file       driver.c
 * @details    Simple Linux device driver (IOCTL)
 * @author     smalinux
 *
 * How to Use: $ ./test_ioctl
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
#include <linux/ioctl.h>
 
 
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
 
int32_t value = 0;
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev sma_cdev;

/*
** Function Prototypes
*/
static int      __init sma_driver_init(void);
static void     __exit sma_driver_exit(void);
static int      sma_open(struct inode *inode, struct file *file);
static int      sma_release(struct inode *inode, struct file *file);
static ssize_t  sma_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  sma_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     sma_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = sma_read,
        .write          = sma_write,
        .open           = sma_open,
        .unlocked_ioctl = sma_ioctl,
        .release        = sma_release,
};

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
static ssize_t sma_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read Function\n");
        return 0;
}

/*
** This fuction will be called when we write the Device file
*/
static ssize_t sma_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write function\n");
        return 0;
}

/*
** This fuction will be called when we write IOCTL on the Device file
*/
static long sma_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        copy_from_user(&value ,(int32_t*) arg, sizeof(value));
                        printk(KERN_INFO "Writing Value = %d\n", value);
                        break;
                case RD_VALUE:
                        copy_to_user((int32_t*) arg, &value, sizeof(value));
                        printk(KERN_INFO "Reading Value = %d\n", value);
                        break;
        }
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
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

/*
** Module exit function
*/
static void __exit sma_driver_exit(void)
{
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
MODULE_DESCRIPTION("Simple Linux device driver (IOCTL)");
MODULE_VERSION("1.5");
