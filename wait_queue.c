#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include<linux/workqueue.h>

static DECLARE_WAIT_QUEUE_HEAD(my_wq);
static int condition = 0;

/* declare a work queue*/
static struct work_struct wrk_t;

static void work_handler(struct work_struct *work)
{
	printk("Waitqueue module handler %s\n", __FUNCTION__);
	msleep(5000);
	printk("Wake up the sleeping module\n");
	condition = 1;
	wake_up_interruptible(&my_wq);
}

static int __init my_init(void)
{
	pr_info("wait_queue: module loaded!\n");

	INIT_WORK(&wrk_t, work_handler);
	schedule_work(&wrk_t);

	printk("Going to sleep %s\n", __FUNCTION__);
	wait_event_interruptible(my_wq, condition != 0);

	pr_info("woken up by the work job\n");

	return 0;
}

void my_exit(void)
{
	pr_info("waitqueue: exit...\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("John Madieu <john.madieu@foobar.com>");
MODULE_LICENSE("GPL");
