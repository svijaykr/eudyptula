#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>


static int hello_open(struct inode *inp, struct file *fp);
static int hello_release(struct inode *inp, struct file *fp);
static ssize_t hello_read(struct file *fp, char __user *user, size_t size, 
			loff_t *offs);


static const struct file_operations hello_fops = {
        .owner = THIS_MODULE,
        .read = hello_read,
	.open = hello_open,
	.release = hello_release,
	};


static struct miscdevice hello_misc = {
        .minor = MISC_DYNAMIC_MINOR,
        .name = "eudyptula",
        .fops = &hello_fops,
	};

static int __init hello_init(void)
{
	int error = 0;
	pr_debug(">> hello_init()\n");
	error = misc_register(&hello_misc);

	if (error) {
		pr_err("can't misc_register err: %d\n", error);
		return error;
	} else{
		pr_info("hello_misc registered");
	}

	return 0;
}
module_init(hello_init);

static void __exit hello_exit(void)
{
	misc_deregister(&hello_misc);
	return;
}
module_exit(hello_exit);


static int hello_open(struct inode *inp, struct file *fp)
{
	pr_info(">> hello_open()");
	return 0;
}

static int hello_release(struct inode *inp, struct file *fp)
{
	pr_info(">> hello_release()");
	return 0;
}

static ssize_t hello_read(struct file *fp, char __user *user, size_t size, 
			loff_t *offs)
{
	pr_info(">> hello_read()");
	return 0;
}

MODULE_LICENSE("GPL");
