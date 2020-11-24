/*
 * @file       export_symbols.c
 * @details    Simple Linux device driver all what he do is to
 * 		share some symbols (EXPORT_SYMBOL)
 * @author     smalinux
 *
 * insert the module then use:
 * 	cat /proc/kallsyms | grep sohaib_
 *
 * In a programming language, a symbol is either a variable or 
 * a function. Or more generally, we can say, a symbol is a name representing 
 * space in the memory, which stores data (variable,
 * for reading and writing) or instructions (function, for executing).
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
int SOHAIB_CONUT = 0;
EXPORT_SYMBOL(SOHAIB_CONUT);

void sohaib_shared_func(void)
{
	printk(KERN_INFO "Shared function been called!!!\n");
	SOHAIB_CONUT++;
}
EXPORT_SYMBOL(sohaib_shared_func);


static int __init smalinux_driver_init(void)
{
	printk("export_symbols: module loaded!!");
	return 0;
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
