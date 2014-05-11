#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

static struct usb_device_id hello_id_table [] = {
	{ USB_INTERFACE_INFO (USB_INTERFACE_CLASS_HID, 
		USB_INTERFACE_SUBCLASS_BOOT, 
		USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{ } /* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, hello_id_table);

static int hello_probe(struct usb_interface *interface,
	const struct usb_device_id *id)
{
	return 0;
}

static void hello_disconnect(struct usb_interface *interface)
{
	return;
}

static struct usb_driver hello_driver = {
	.name =     "hello_driver",
	.probe =    hello_probe,
	.disconnect =   hello_disconnect,
	.id_table = hello_id_table
};

static int __init hello_init(void)
{
	int result;
	result = usb_register(&hello_driver);
	if(result)
		pr_debug("Hello World!\n");
	return 0;

}

static void __exit hello_exit(void)
{
	usb_deregister(&hello_driver);
	return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
