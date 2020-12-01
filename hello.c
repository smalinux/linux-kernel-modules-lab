/*
 * @file	hello.c
 * @details	Simple linux kernel module
 * @author	smalinux
 *
 */
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("smalinux <xunilams@gmail.com>");

static int hello_init(void)
{
	pr_err("Hello, world\n");
	return 0;
}

static void hello_exit(void)
{
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
