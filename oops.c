#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

static noinline void do_oops(void)
{
    *(int*)0x42 = 'a';
}

static int so2_oops_init(void)
{
    pr_info("oops_init\n");
    do_oops();

    return 0;
}

static void so2_oops_exit(void)
{
    pr_info("oops exit\n");
}

module_init(so2_oops_init);
module_exit(so2_oops_exit);

/** MODULE_
*/
MODULE_AUTHOR("Sohaib (smalinux) <xunilams@gmail.com>");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("hello Description");
// MODULE_ALIAS();
// MODULE_DEVICE_TABLE();
MODULE_LICENSE("GPL");
