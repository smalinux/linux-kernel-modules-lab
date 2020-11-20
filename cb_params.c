/*
 * cb_params.c - Demonstrates command line argument passing to a module.
 *
 * $insmod params.ko myint=100 
 * $echo 200 > /sys/module/params/parameters/myint
 * $dmesg
 */

#include <linux/module.h>
//#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sohaib <xunilams@gmail.com>");

static int myint = 420;

/*----------------------Module_param_cb()--------------------------------*/
int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                printk(KERN_INFO "Call back function called...\n");
                printk(KERN_INFO "New value of myint = %d\n", myint);
                return 0;
        }
        return 0;
}

const struct kernel_param_ops my_param_ops =
{
        .set = &notify_param, // Use our setter ...
        .get = &param_get_int, // .. and standard getter
};

module_param_cb(myint, &my_param_ops, &myint, S_IRUGO|S_IWUSR );
/*-------------------------------------------------------------------------*/


static int hello_init(void)
{
	pr_cont("params.ko: module loaded!");
	printk(KERN_INFO "myint is an integer: %d\n", myint);

	return 0;
}

static void hello_exit(void)
{
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
