/*
 * @file       trick_kallysms.c
 *
 * I made a very nice trick :D see trick_kallysms.c module.
 * After inserting export_symbols.c module I took the address 
 * of SOHAIB_CONUT symbol from /proc/kallsyms file, then
 * I used it as a hard code in trick_kallysms.c :))
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
	int * p;
	/*
	 *
	 * You need to get the address of SOHAIB_CONUT from
	 *  cat /proc/kallsyms | grep SOHAIB_
	 * and assign it to pointer p :))
	 * you could make it dynamically:
	 *  p = &SOHAIB_CONUT;
	 *
	 * */
	p = 0xffffffffc1a0f400;		// p = &SOHAIB_CONUT;
	*p = 500;

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
