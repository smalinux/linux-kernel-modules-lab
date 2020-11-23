/*
 * @file       export_symbols.c
 * @details    Simple Linux device driver all what he do is to
 * 		share some symbols (EXPORT_SYMBOL)
 * @author     smalinux
 *
 * insert the module then use:
 * 	cat /proc/kallsyms | grep sma_
 *
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

/**
 * Declarations
 *****************************************************/
int sma_count = 0;

void sma_shared_func(void)
{
	printk(KERN_INFO "Shared function been called!!!\n");
	sma_count++;
}


EXPORT_SYMBOL(sma_count);
EXPORT_SYMBOL(sma_shared_func);


static int __init smalinux_driver_init(void)
{
	printk("export_symbols: module loaded!!");
}
 
static void __exit smalinux_driver_exit(void)
{
	pr_info("export_symbols: module unloaded!!");
}

/**
 * Definitions
 *****************************************************/
 
module_init(smalinux_driver_init);
module_exit(smalinux_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("smalinux <xunilams@gmail.com>");
MODULE_DESCRIPTION("EXPORT_SYMBOL Driver - share some symbols");
MODULE_VERSION("1.2");
