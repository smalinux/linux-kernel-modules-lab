#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
MODULE_LICENSE("GPL");

#define BUFSIZE  100
char buf[BUFSIZE];

static struct proc_dir_entry *ent;

/* cat /proc/proc_fs */
static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	int len=0;
	//char buf[BUFSIZE];
	printk( KERN_DEBUG "read handler\n");
	if(*ppos > 0 || count < BUFSIZE)
		return 0;
	
	if(copy_to_user(ubuf,buf,len))
		return -EFAULT;
	printk( KERN_DEBUG "%s\n", buf);
	*ppos = len;
	return len;
}

/* echo "test" > /proc/proc_fs */
static ssize_t mywrite(struct file *file, const char __user *ubuf,size_t count, loff_t *ppos)
{
	int c;
	//char buf[BUFSIZE];
	if(*ppos > 0 || count > BUFSIZE)
		return -EFAULT;
	if(copy_from_user(buf,ubuf,count))
		return -EFAULT;
	c = strlen(buf);
	*ppos = c;
	return c;
}

static struct proc_ops myops =
{
	.proc_read 	= myread,
	.proc_write 	= mywrite,
	.proc_lseek	= noop_llseek,
};

static int hello_init(void)
{
	pr_info("proc_fs: module loaded\n");
	ent = proc_create("proc_fs_1", 0660, NULL, &myops);
	return 0;
}

static void hello_exit(void)
{
	proc_remove(ent);
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
