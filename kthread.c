/*
 * @file       kthread.c
 * @details    Simple Linux device driver (Kernel Thread)
 * @author     smalinux
 *
 * Minimal Sample:
 * 	https://github.com/agelastic/eudyptula/blob/master/task17/helloworld.c
 *
 * Google: linux kernel threads documentation
 * https://stackoverflow.com/a/10178410/5688267
 * https://stackoverflow.com/a/36339961/5688267
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/kthread.h>             //kernel threads
#include <linux/sched.h>               //task_struct 
#include <linux/delay.h>

dev_t dev = 0;
static struct class *dev_class;
static struct cdev sma_cdev;

static int __init sma_driver_init(void);
static void __exit sma_driver_exit(void);

static struct task_struct *sma_thread;

/*
** Function Prototypes
*/
/*************** Driver Fuctions **********************/
static int sma_open(struct inode *inode, struct file *file);
static int sma_release(struct inode *inode, struct file *file);
static ssize_t sma_read(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t sma_write(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
 /******************************************************/
 
int thread_function(void *pv);

/*
** Thread
*/
int thread_function(void *pv)
{
    int i=0;
    while(!kthread_should_stop()) {
        printk(KERN_INFO "In Thread Function %d\n", i++);
        msleep(1000);
    }
    return 0;
}


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
        return len;
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
            printk(KERN_INFO "Cannot create the Device \n");
            goto r_device;
        }
 
        sma_thread = kthread_create(thread_function,NULL,"SMA Thread");
        if(sma_thread) {
            wake_up_process(sma_thread);
        } else {
            printk(KERN_ERR "Cannot create kthread\n");
            goto r_device;
        }
#if 0
        /* You can use this method to create and run the thread */
        sma_thread = kthread_run(thread_function,NULL,"SMA Thread");
        if(sma_thread) {
            printk(KERN_ERR "Kthread Created Successfully...\n");
        } else {
            printk(KERN_ERR "Cannot create kthread\n");
             goto r_device;
        }
#endif
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
        return 0;
 
 
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
        kthread_stop(sma_thread);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&sma_cdev);
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "Device Driver Remove...Done!!\n");
}
 
module_init(sma_driver_init);
module_exit(sma_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("smalinux <xunilams@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - Kernel Thread");
MODULE_VERSION("1.14");
