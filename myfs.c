/*
 * @file	myfs.c
 * @details	a simple, virtual file system driver(i.e. without physical disk support).
 * @author	smalinux
 *
 * I try to get familiar with the interface exposed by the Linux kernel 
 * and the Virtual File System (VFS) component.
 *
 * Further reading:
 * * Documentation/filesystems/vfs.rst
 * * https://github.com/smalinux/linux-kernel-labs.github.io/blob/master/tmp/_sources/labs/filesystems_part1.rst.txt
 * * https://github.com/smalinux/linux-kernel-labs.github.io/blob/master/tmp/_sources/labs/filesystems_part2.rst.txt
 * * https://www.cs.ou.edu/~fagg/classes/cs3113_f20/projects/project3.html
 * * 
 * * Robert Love – Linux Kernel Development, Second Edition – Chapter 12. The Virtual Filesystem
 * * Understanding the Linux Kernel, 3rd edition - Chapter 12. The Virtual Filesystem
 * * https://www.win.tue.nl/~aeb/linux/lk/lk-8.html
 * * http://inglorion.net/documents/tutorials/tutorfs/
 * * http://www.linux.it/~rubini/docs/vfs/vfs.html
 * * http://www.linux.it/~rubini/docs/vfs/vfs.html
 * * https://lwn.net/Articles/57369/
 * * https://www.cyberciti.biz/tips/understanding-unixlinux-file-system-part-i.html
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/pagemap.h>

MODULE_DESCRIPTION("Simple virtual filesystem");
MODULE_AUTHOR("smalinux");
MODULE_LICENSE("GPL");


#define MYFS_BLOCKSIZE		4096
#define MYFS_BLOCKSIZE_BITS	12
#define MYFS_MAGIC		0xbeefcafe
#define LOG_LEVEL		KERN_ALERT

/* declarations of functions that are part of operation structures */

static int myfs_mknod(struct inode *dir,
		struct dentry *dentry, umode_t mode, dev_t dev);
static int myfs_create(struct inode *dir, struct dentry *dentry,
		umode_t mode, bool excl);
static int myfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode);

/* define super_operations structure */
static struct super_operations myfs_super_operations = {

};

/* Fill dir inode operations structure. */
static const struct inode_operations myfs_dir_inode_operations = {
	.create		= myfs_create,
	.lookup		= simple_lookup,
	.link		= simple_link,
	.mkdir		= myfs_mkdir,
	.rmdir		= simple_rmdir,
	.unlink		= simple_unlink,
	.mknod		= myfs_mknod,
	.rename		= simple_rename,
};

/* Fill file operations structure. */
static const struct file_operations myfs_file_operations = {
	.read_iter	= generic_file_read_iter,
	.write_iter	= generic_file_write_iter,
	.mmap		= generic_file_mmap,
	.fsync		= noop_fsync,
	.splice_read	= generic_file_splice_read,
	.splice_write	= iter_file_splice_write,
	.llseek		= generic_file_llseek,
};

/* Fill file inode operations structure. */
static const struct inode_operations myfs_file_inode_operations = {
	.setattr	= simple_setattr,
	.getattr	= simple_getattr,
};

/* Fill address space operations structure. */
static const struct address_space_operations myfs_aops = {
	.readpage	= simple_readpage,
	.write_begin	= simple_write_begin,
	.write_end	= simple_write_end,
};

struct inode *myfs_get_inode(struct super_block *sb, const struct inode *dir,
		int mode)
{
	struct inode *inode = new_inode(sb);

	if (!inode)
		return NULL;

	/* fill inode structure */
	inode_init_owner(inode, NULL, mode);
	inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);
	inode->i_ino = get_next_ino();

	/* Initialize address space operations. */
	inode->i_mapping->a_ops = &myfs_aops;

	if (S_ISDIR(mode)) {
		/* set inode operations for dir inodes. */
		inode->i_op = &myfs_dir_inode_operations;
		inode->i_fop = &simple_dir_operations;

		/* directory inodes start off with i_nlink == 2 (for "." entry).
		 * Directory link count should be incremented (use inc_nlink).
		 */
		inc_nlink(inode);
	}

	/* Set file inode and file operations for regular files
	 * (use the S_ISREG macro).
	 */
	if (S_ISREG(mode)) {
		inode->i_op = &myfs_file_inode_operations;
		inode->i_fop = &myfs_file_operations;
	}

	return inode;
}

/* Implement myfs_mknod, myfs_create, myfs_mkdir. */

static int myfs_mknod(struct inode *dir,
		struct dentry *dentry, umode_t mode, dev_t dev)
{
	struct inode * inode = myfs_get_inode(dir->i_sb, dir, mode);
	int error = -ENOSPC;

	if (inode) {
		d_instantiate(dentry, inode);
		dget(dentry);	/* Extra count - pin the dentry in core */
		error = 0;
		dir->i_mtime = dir->i_ctime = current_time(dir);
	}
	return error;
}

static int myfs_create(struct inode *dir, struct dentry *dentry,
		umode_t mode, bool excl)
{
	return myfs_mknod(dir, dentry, mode | S_IFREG, 0);
}

static int myfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	int retval = myfs_mknod(dir, dentry, mode | S_IFDIR, 0);
	if (!retval)
		inc_nlink(dir);
	return retval;
}

static int myfs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode *root_inode;
	struct dentry *root_dentry;

	/* fill super_block
	 *   - blocksize, blocksize_bits
	 *   - magic
	 *   - super operations
	 *   - maxbytes
	 */
	sb->s_blocksize 	= MYFS_BLOCKSIZE;
	sb->s_blocksize_bits	= MYFS_BLOCKSIZE_BITS;
	sb->s_magic		= MYFS_MAGIC;
	sb->s_op		= &myfs_super_operations;
	sb->s_maxbytes		= MAX_LFS_FILESIZE;

	/* mode = directory & access rights (755) */
	root_inode = myfs_get_inode(sb, NULL,
			S_IFDIR | S_IRWXU | S_IRGRP |
			S_IXGRP | S_IROTH | S_IXOTH);

	if (!root_inode)
		return -ENOMEM;

	pr_err("root inode has %d link(s)\n", root_inode->i_nlink);

	root_dentry = d_make_root(root_inode);
	if (!root_dentry)
		goto out_no_root;
	sb->s_root = root_dentry;

	return 0;

out_no_root:
	iput(root_inode);
	return -ENOMEM;
}

static struct dentry *myfs_mount(struct file_system_type *fs_type,
		int flags, const char *dev_name, void *data)
{
	/* extern struct dentry *mount_nodev(struct file_system_type *fs_type,
	int flags, void *data,
	int (*fill_super)(struct super_block *, void *, int));*/
	pr_err("ok, I will mount");
	return mount_nodev(fs_type, flags, data, myfs_fill_super);
}

static void myfs_kill_sb(struct super_block *sb)
{
	/* kill_anon_super(struct super_block *sb); */
	kfree(sb->s_fs_info);
	kill_litter_super(sb);
	pr_err("umount, i have killed myself...");
}

/* define file_system_type structure */
static struct file_system_type myfs_system_type = {
	.owner		= THIS_MODULE,
	.name 		= "myfs",
	.mount		= myfs_mount,
	.kill_sb	= myfs_kill_sb,
};

static int __init myfs_init(void)
{
	int err;

	/* register */
	err = register_filesystem(&myfs_system_type);
	if (err) {
		pr_err("register_filesystem failed\n");
		return err;
	}

	return 0;
}

static void __exit myfs_exit(void)
{
	/* unregister */
	unregister_filesystem(&myfs_system_type);
}

module_init(myfs_init);
module_exit(myfs_exit);
