/*
 * @file	misc_driver.c
 * @details	Simple misc driver explanation
 * @author	smalinux
 *
 * What misc driver mean?! 
 * What is the difference between misc drivers and char drivers?!
 *
 * the misc API seems to make your life easier when you're writing a small
 * character driver and do not want to need to allocate a new major number
 * only to use one minor number, for example. It simplifies things, but 
 * all the file operations are still available using the fops member of 
 * struct miscdevice. The basic difference is you only get one minor
 * number per misc device.
 *
 * link:https://stackoverflow.com/a/18456796/5688267
 */
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
 
static int sma_misc_open(struct inode *inode, struct file *file)
{
    pr_info("SMA misc device open\n");
    return 0;
}
 
static int sma_misc_close(struct inode *inodep, struct file *filp)
{
    pr_info("SMA misc device close\n");
    return 0;
}
 
static ssize_t sma_misc_write(struct file *file, const char __user *buf,
               size_t len, loff_t *ppos)
{
    pr_info("SMA misc device write\n");
    
    /* We are not doing anything with this data now */
    
    return len; 
}
 
 
static ssize_t sma_misc_read(struct file *filp, char __user *buf,
                    size_t count, loff_t *f_pos)
{
    pr_info("SMA misc device read\n");
 
    return 0;
}
 
static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .write          = sma_misc_write,
    .read           = sma_misc_read,
    .open           = sma_misc_open,
    .release        = sma_misc_close,
    .llseek         = no_llseek,
};
 
struct miscdevice sma_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "simple_sma_misc",
    .fops = &fops,
};
 
/*
 *
 */
static int __init misc_init(void)
{
    int error;
 
    error = misc_register(&sma_misc_device);
    if (error) {
        pr_err("misc_register failed!!!\n");
        return error;
    }
 
    pr_info("misc_register init done!!!\n");
    return 0;
}
 
static void __exit misc_exit(void)
{
    misc_deregister(&sma_misc_device);
    pr_info("misc_register exit done!!!\n");
}
 
module_init(misc_init)
module_exit(misc_exit)
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("smalinux <xunilams@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - Misc Driver");
MODULE_VERSION("1.29");
