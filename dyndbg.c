#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/errno.h>

#define MEM_SIZE        1024           //Memory Size
uint8_t *kernel_buffer;

static ssize_t hello_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
        //Copy the data from the kernel space to the user-space
        copy_to_user(buf, kernel_buffer, MEM_SIZE);
        pr_debug("Data Read : Done!\n");
        return MEM_SIZE;
}

static ssize_t hello_write(struct file *file, char const __user *buf,
			size_t count, loff_t *ppos)
{
        //Copy the data to kernel space from the user-space
        pr_debug("Data Write : Done!\n");
        copy_from_user(kernel_buffer, buf, count);
        return count;
}

static const struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.read = hello_read,
	.write = hello_write,
};

static struct miscdevice hello_dev = {
	MISC_DYNAMIC_MINOR,
	"dyndbg",
	&hello_fops
};

static int __init hello_init(void)
{
	int ret;

	ret = misc_register(&hello_dev);
	pr_info("Hello World!\n");

	return ret;
}

static void __exit hello_exit(void)
{
	pr_info("Bye World!\n");
	misc_deregister(&hello_dev);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("smalinux <xunilams@gmail.com>");
MODULE_DESCRIPTION("dyndbg");
