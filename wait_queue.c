/*
   Whenever a process must wait for an event (such as the arrival of data or the termination of a process), it should go to sleep. Sleeping causes the process to suspend execution, freeing the processor for other uses. After some time, the process will be woken up and will continue with its job when the event which we are waiting for has arrived.

   Wait queue is a mechanism provided in the kernel to implement the wait. As the name itself suggests, waitqueue is the list of processes waiting for an event. In other words, A wait queue is used to wait for someone to wake you up when a certain condition is true. They must be used carefully to ensure there is no race condition.

   There are 3 important steps in Waitqueue
    1. Initializing Waitqueue
    2. Queuing (Put the Task to sleep until the event comes)
    3. Waking Up Queued Task

  Queue APIs (sleeping)
    1. wait_event
    2. wait_event_timeout
    3. wait_event_cmd
    4. wait_event_interruptible
    5. wait_event_interruptible_timeout
    6. wait_event_killable

  Waking Up Queued Task
    1. wake_up
    2. wake_up_all
    3. wake_up_interruptible
    4. wake_up_sync
    5. wake_up_interruptible_sync

Farther reading:
    1. https://web.archive.org/web/20200815075526/https://embetronicx.com/tutorials/linux/device-drivers/waitqueue-in-linux-device-driver-tutorial/
    2. 
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

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
