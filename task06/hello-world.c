#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/errno.h>

static int hello_open(struct inode *inp, struct file *fp);
static int hello_release(struct inode *inp, struct file *fp);
static ssize_t hello_read(struct file *fp, char __user *user, size_t size, 
			loff_t *offs);

static ssize_t hello_write(struct file *fp, const char __user *user, size_t size,
			loff_t *offs);

static const struct file_operations hello_fops = {
        .owner = THIS_MODULE,
        .read = hello_read,
	.open = hello_open,
	.release = hello_release,
	.write = hello_write
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


int hello_open(struct inode *inp, struct file *fp)
{
	pr_debug(">> hello_open()");
	return 0;
}

int hello_release(struct inode *inp, struct file *fp)
{
	pr_debug(">> hello_release()");
	return 0;
}

ssize_t hello_read(struct file *fp, char __user *user, size_t size, 
			loff_t *offs)
{
	char buf[4] = "";
	pr_debug(">> hello_read()");
	sprintf(buf,"%d\n", hello_misc.minor);
	pr_debug("Sending %s", buf);

	if((*offs) == (strlen(buf) + 1))
		return 0;
	else if(!copy_to_user(user, buf, (strlen(buf) + 1)))
		return ((*offs) += (strlen(buf) + 1));
	else
		return EAGAIN;
}

ssize_t hello_write(struct file *fp, const char __user *user, size_t size,
			loff_t *offs)
{
	char buf_mine[4] = "";
	char buf_arg[4] = "";
	pr_debug(">> hello_write()");
	sprintf(buf_mine, "%d", hello_misc.minor);
	pr_debug("size:%lu ", size);
	pr_debug("offset:%lld ", (*offs));

	if ((strlen(buf_mine) == (size - 1)) && 
		!copy_from_user(buf_arg, user, strlen(buf_mine) + 1)){
		pr_debug("received: %s", buf_arg);
		pr_debug("mine: %s", buf_mine);
		return strncmp(buf_mine, buf_arg, strlen(buf_mine)) ? -EINVAL :
			size;
	} else
		return -EINVAL;
}

MODULE_LICENSE("GPL");
