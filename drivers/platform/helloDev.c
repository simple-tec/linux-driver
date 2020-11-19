#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/slab.h>

#define BUFFER_MAX    (10)
#define OK            (0)
#define ERROR         (-1)

struct cdev *gDev;
struct file_operations *gFile;
dev_t  devNum;
unsigned int subDevNum = 1;
int reg_major  =  232;    
int reg_minor =   0;
char *buffer;
#define LEDBASE	0x56000010
#define LEDLEN	0x0c

int hello_open(struct inode *p, struct file *f)
{
    printk(KERN_INFO "hello_open\r\n");
    return 0;
}

ssize_t hello_write(struct file *f, const char __user *u, size_t s, loff_t *l)
{
    printk(KERN_INFO "hello_write\r\n");
    return 0;
}
ssize_t hello_read(struct file *f, char __user *u, size_t s, loff_t *l)
{
    printk(KERN_INFO "hello_read\r\n");      
    return 0;
}

static int  hellodev_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "hellodev_probe\n");
    devNum = MKDEV(reg_major, reg_minor);
    if(OK == register_chrdev_region(devNum, subDevNum, "helloworld")){
        printk(KERN_INFO "register_chrdev_region ok \n"); 
    }else {
    printk(KERN_INFO "register_chrdev_region error n");
        return ERROR;
    }
    printk(KERN_INFO " hello driver init \n");
    gDev = kzalloc(sizeof(struct cdev), GFP_KERNEL);
    gFile = kzalloc(sizeof(struct file_operations), GFP_KERNEL);
    gFile->open = hello_open;
    gFile->read = hello_read;
    gFile->write = hello_write;
    gFile->owner = THIS_MODULE;
    cdev_init(gDev, gFile);
    cdev_add(gDev, devNum, 1);
    return 0;
}
static int hellodev_remove(struct platform_device *pdev)
{
    printk(KERN_INFO "hellodev_remove \n");
    cdev_del(gDev);
    kfree(gFile);
    kfree(gDev);
    unregister_chrdev_region(devNum, subDevNum);
    return;
}

static void hello_plat_release(struct device *dev)
{
	return;
}
static struct resource hello_dev_resource[] = {
	[0] = {
		.start = LEDBASE,
		.end   = LEDBASE + LEDLEN - 1,
		.flags = IORESOURCE_MEM,
	}
};

struct platform_device hello_device = {
	.name		  = "hello-device",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(hello_dev_resource),
	.resource	  = hello_dev_resource,
	.dev = {
		.release = hello_plat_release,
    }
};
static struct platform_driver hellodev_driver = {
	.probe		= hellodev_probe,
	.remove		= hellodev_remove,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "hello-device",
	},
};

int charDrvInit(void)
{
	platform_device_register(&hello_device);
    return platform_driver_register(&hellodev_driver); 
}

void __exit charDrvExit(void)
{
	platform_device_unregister(&hello_device);
    platform_driver_unregister(&hellodev_driver);
    return;
}


module_init(charDrvInit);

module_exit(charDrvExit);

MODULE_LICENSE("GPL");

