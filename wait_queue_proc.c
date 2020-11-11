#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#define BUFSIZE  100


static DECLARE_WAIT_QUEUE_HEAD(my_wq);
static int condition = 0;
/* declare a proc_fs ent */
static struct proc_dir_entry *ent;

/* declare a work queue */
static struct work_struct wrk_t;

static void work_handler(struct work_struct *work)
{
	printk("Waitqueue module handler %s\n", __FUNCTION__);
}

static ssize_t mywrite(struct file *file, const char __user *ubuf,size_t count, loff_t *ppos)
{
	if(wq_has_sleeper(&my_wq)) {
		printk( KERN_DEBUG "write handler\n");
		printk("Wake up the sleeping module\n");
		condition = 1;
		wake_up_interruptible(&my_wq);
	}
	return -1;
}

static struct proc_ops myops =
{
	.proc_write 	= mywrite,
};

static int __init my_init(void)
{
	pr_info("wait_queue: module loaded!\n");
	ent = proc_create("mydev", 0660, NULL, &myops);	// create proc_fs
	
	INIT_WORK(&wrk_t, work_handler);
	schedule_work(&wrk_t);

	printk("Going to sleep %s\n", __FUNCTION__);
	wait_event_interruptible(my_wq, condition != 0);

	pr_info("woken up by the work job\n");

	return 0;
}

void my_exit(void)
{
	proc_remove(ent);
	pr_info("waitqueue: exit...\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("John Madieu <john.madieu@foobar.com>");
MODULE_LICENSE("GPL");
