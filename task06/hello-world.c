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

#define MINOR_SIZE 13

static char minor[MINOR_SIZE] = "123456789012";

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
	return simple_read_from_buffer(user, size, offs, minor, strlen(minor));
}

ssize_t hello_write(struct file *fp, const char __user *user, size_t size,
			loff_t *offs)
{
	char tmp[MINOR_SIZE];
	int result = 0;

	if (size == MINOR_SIZE) {
		result = simple_write_to_buffer(tmp, MINOR_SIZE-1,  offs, user,
						size) + 1;
		tmp[MINOR_SIZE - 1] = '\0';
	} else
		result = -EINVAL;

	if ((*offs) == strlen(minor))
		result = strncmp(tmp, minor, strlen(tmp)) ? -EINVAL : result;

	return result;
}

MODULE_LICENSE("GPL");
