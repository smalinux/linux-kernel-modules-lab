/*
 * @file	debug.c
 * @details	Pinting the line of code, file name, function name ..
 * @author	smalinux
 *
 */
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("smalinux <xunilams@gmail.com>");

#define PRINT_DEBUG \
       printk (KERN_DEBUG "[%s]: FUNC:%s: LINE:%d \n", __FILE__, \
		       __FUNCTION__, __LINE__)

static int hello_init(void)
{
	PRINT_DEBUG;
	pr_err("Hello, world\n");
	PRINT_DEBUG;
	return 0;
}

static void hello_exit(void)
{
	PRINT_DEBUG;
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
