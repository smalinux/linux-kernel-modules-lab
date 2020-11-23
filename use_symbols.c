/*
 * @file       use_symbols.c
 * @details    Simple Linux device driver uses symbols of 
 * 		other module ( export_symbols.c )
 * @author     smalinux
 *
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

extern int SOHAIB_CONUT;
void sohaib_shared_func(void);

static int __init smalinux_driver_init(void) 
{
	printk("use_symbols: module loaded!!");
	sohaib_shared_func();
	printk("sohaib_count: %d", SOHAIB_CONUT);

	return 0;
}
 
static void __exit smalinux_driver_exit(void)
{
	pr_info("use_symbols: module unloaded!!");
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
