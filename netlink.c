/*
 * file       sys_fs.c
 * details    Simple Linux device driver (sysfs)
 * author     smalinux
 *
 * https://github.com/cirosantilli/linux-kernel-module-cheat/tree/e05eb91addeb30a8486fa0926270a36aafe58fe7#netlink-sockets
 *
 * https://stackoverflow.com/questions/3299386/how-to-use-netlink-socket-to-communicate-with-a-kernel-module
 *
 *
 */
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");

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
