/*
 * @file	timer.c
 * @details	Simple Linux device driver (Kernel Timer)
 * @author	smalinux
 *
 * struct timer_list {
 * 	struct list_head entry;		// entry in linked list of timers
 *	unsigned long expires;		// expiration value, in jiffies
 *	void (*function)(unsigned long);  // the timer handler function
 *	unsigned long data;		// lone argument to the handler
 *	struct tvec_t_base_s *base;	// internal timer field, do not touch
 * }
 *
 * References:
 * 	Love's book ch 11
 * 	LDD3 ch 7
 * 	ULK book
 * 	GitHub 0xAX linux-insides
 * 	Google: jiffies
 * 	Documentation/timers/index.rst
 *	https://lwn.net/Articles/735887/
 *
 * Minimal Samples:
 * 	https://stackoverflow.com/a/63679715/5688267
 * 	https://gist.github.com/smalinux/b65ef70b5866c1192e3e5ba36d86f0f1
 *
 * Cute Tutorials:
 * 	https://embetronicx.com/tutorials/linux/device-drivers/using-kernel-timer-in-linux-device-driver/
 * 	https://embetronicx.com/tutorials/linux/device-drivers/using-high-resolution-timer-in-linux-device-driver/
 *
 *
 * What driver do?
 *	1. Initialize the timer and set the time interval
 *	2. After the timeout, a registered timer callback will be called.
 *	3. In the timer callback function again we are re-enabling the timer.
 *	    We have to do this step if we want a periodic timer. 
 *	    Otherwise, we can ignore this.
 *	4. Once we have done, we can disable the timer.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
 
//Timer Variable
#define TIMEOUT 5000    //milliseconds
static struct timer_list sma_timer;
static unsigned int count = 0;
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev sma_cdev;
 
/* declarations of functions  */
 
static int __init sma_driver_init(void);
static void __exit sma_driver_exit(void);
static int sma_open(struct inode *inode, struct file *file);
static int sma_release(struct inode *inode, struct file *file);
static ssize_t sma_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t sma_write(struct file *filp, const char *buf, size_t len, loff_t * off);
 
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = sma_read,
        .write          = sma_write,
        .open           = sma_open,
        .release        = sma_release,
};
 
//Timer Callback function. This will be called when timer expires
void timer_callback(struct timer_list * data)
{
     /* do your timer stuff here */
    printk(KERN_INFO "Timer Callback function Called [%d]\n",count++);
    
    /*
       Re-enable timer. Because this function will be called only first time. 
       If we re-enable this will work like periodic timer. 
    */
    mod_timer(&sma_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}
 
static int sma_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device File Opened...!!!\n");
    return 0;
}
 
static int sma_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device File Closed...!!!\n");
    return 0;
}
 
static ssize_t sma_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read Function\n");
    return 0;
}
static ssize_t sma_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Write function\n");
    return 0;
}
 
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
 
    /* setup your timer to call my_timer_callback */
    timer_setup(&sma_timer, timer_callback, 0);       //If you face some issues and using older kernel version, then you can try setup_timer API(Change Callback function's argument to unsingned long instead of struct timer_list *.
 
    /* setup timer interval to based on TIMEOUT Macro */
    mod_timer(&sma_timer, jiffies + msecs_to_jiffies(TIMEOUT));
 
    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev,1);
    return -1;
}
 
static void __exit sma_driver_exit(void)
{
    /* remove kernel timer when unloading module */
    del_timer(&sma_timer);
 
    class_destroy(dev_class);
    cdev_del(&sma_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(sma_driver_init);
module_exit(sma_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("smalinux <xunilams@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - Kernel Timer");
MODULE_VERSION("1.21");

/**

Timers are another type of "bottom half" that are used to schedule the execution of future actions after a certain amount of time has passed.  

Introduction
=======================================
What is a timer in general? According to Wikipedia, A timer is a specialized type of clock used for measuring specific time intervals. Timers can be categorized into two main types. A timer that counts upwards from zero for measuring elapsed time is often called a stopwatch, while a device that counts down from a specified time interval is more usually called a timer.

Timer in Linux Kernel
=======================================
In Linux, kernel keeps track of the flow of time by means of timer interrupts. This timer interrupts are generated at regular timer intervals by using system’s timing hardware. Every time a timer interrupt occurs, the value of an internal kernel counter is incremented. The counter is initialized to 0 at system boot, so it represents the number of clock ticks since last boot.

Kernel timer offers less precision but is more efficient in situations where the timer will probably be canceled before it fires. There are many places in the kernel where timers are used to detect when a device or a network peer has failed to respond within the expected time.

When you want to do some action after some time, then kernel timers are one of the options for you. These timers are used to schedule the execution of a function at a particular time in the future, based on the clock tick, and can be used for a variety of tasks.

Uses of Kernel Timers
=====================================
    * Polling a device by checking its state at regular intervals when the hardware can’t fire interrupts.
    * The user wants to send some messages to another device at regular intervals.
    * Send error when some action didn’t happen in a particular time period.
    Etc.

Kernel Timer API
=====================================
Linux Kernel provides the driver to create timers that are not periodic by default, register the timers and delete the timers.

We need to include the <linux/timer.h> (#include <linux/timer.h>) in order to use kernel timers. Kernel timers are described by the timer_list structure, defined in <linux/timer.h>:

struct timer_list {
    unsigned long expires;
    void (*function)(unsigned long);
    unsigned long data;
};

The expires field contains the expiration time of the timer (in jiffies).
On expiration, function() will be called with the given data value.
*/
