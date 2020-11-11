#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
MODULE_LICENSE("GPL");

static int hello_init(void)
{
	pr_info("msleep: Please wait 5s seconds ...");
	msleep(1000);
	pr_err("1\n");
	msleep(1000);
	pr_err("2\n");
	msleep(1000);
	pr_err("3\n");
	msleep(1000);
	pr_err("4\n");
	msleep(1000);
	pr_err("Hello, world\n");
	return 0;
}

static void hello_exit(void)
{
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
