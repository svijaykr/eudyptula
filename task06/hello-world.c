#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/uaccess.h>

static ssize_t hello_read(struct file *fp, char __user *user, size_t size,
			loff_t *offs);

static ssize_t hello_write(struct file *fp, const char __user *user,
			size_t size, loff_t *offs);

static const struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.read = hello_read,
	.write = hello_write
};

static struct miscdevice hello_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &hello_fops,
};

static int __init hello_init(void)
{
	return misc_register(&hello_misc);
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	misc_deregister(&hello_misc);
}
module_exit(hello_exit);

ssize_t hello_read(struct file *fp, char __user *user, size_t size,
			loff_t *offs)
{
	char buf[4] = "";
	int len = sprintf(buf, "%d\n", hello_misc.minor) + 1;
	return (*offs) ? 0 : ((*offs) = copy_to_user(user, buf, len) + len);
}

ssize_t hello_write(struct file *fp, const char __user *user, size_t size,
			loff_t *offs)
{
	char buf_mine[4] = "";
	char buf_arg[4] = "";
	sprintf(buf_mine, "%d", hello_misc.minor);

	if ((strlen(buf_mine) == (size - 1)) &&
		!copy_from_user(buf_arg, user, strlen(buf_mine) + 1)) {
		return strncmp(buf_mine, buf_arg, strlen(buf_mine)) ? -EINVAL :
			size;
	} else
		return -EINVAL;
}

MODULE_LICENSE("GPL");
