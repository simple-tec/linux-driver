/*****************************************************************************
简    述：简单字符型驱动程序，测试gpio
******************************************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <asm/io.h>

#define BUFFER_MAX    (10)

#define LED_ENABLE    (0)
#define LED_DISABLE   (1)

#define OK            (0)
#define ERROR         (-1)

#define LEDBASE	0x56000010
#define LEDLEN	0x0c

#define GPSIZE        (0x4)


struct cdev *gDev;
struct file_operations *gFile;
dev_t  devNum;
unsigned int subDevNum = 1;
static void __iomem	*led_base;
int reg_major  =  232;    
static struct resource	*wdt_mem;
int reg_minor =   0;

unsigned long led_time = 1;/* 闪灯的时间间隔，最小为1秒(默认) */ 

int testOpen(struct inode *p, struct file *f)
{
    return OK;
}


int testWrite(struct file *f, const char __user *u, size_t s, loff_t *l)
{
    printk(KERN_EMERG"testWrite\r\n");
    return OK;
}

int testRead(struct file *f, char __user *u, size_t s, loff_t *l)
{
    printk(KERN_EMERG"testRead\r\n");
    return OK;
}
//turun on or off the led 1
int testIoctl(struct inode *node, struct file *f, unsigned int cmd, unsigned long arg)
{
    return 0;
}
static int __devinit s3c2410led_probe(struct platform_device *pdev)
{

    struct resource *res;	
    int size;
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (res == NULL) {
		//dev_err(dev, "no memory resource specified\n");
		return -ENOENT;
    }
    size = (res->end - res->start) + 1;//8
    printk(KERN_INFO "start: %x, len:%x\n", res->start, size);
    /*
    wdt_mem = request_mem_region(res->start, size, pdev->name);
    if (wdt_mem == NULL) {
		dev_err(dev, "failed to get memory region\n");
		ret = -ENOENT;
		goto err_req;
    }*/

    //led_base = ioremap(res->start, size);

	
    devNum = MKDEV(reg_major, reg_minor);

    printk(KERN_EMERG"devNum is 0x%x\r\n", devNum);

    if(OK == register_chrdev_region(devNum, subDevNum, "testchar"))
        {
        printk(KERN_EMERG"register_chrdev_region ok\r\n");
        }
    else
        {
        printk(KERN_EMERG"register_chrdev_region error\r\n");
        return ERROR;
        }
        
    gDev = kzalloc(sizeof(struct cdev), 0);

    gFile = kzalloc(sizeof(struct file_operations), 0);

    gFile->open = testOpen;
    gFile->read = testRead;
    gFile->write = testWrite;
    //gFile->ioctl = testIoctl;
    gDev->owner = THIS_MODULE;
    
    cdev_init(gDev, gFile);
    cdev_add(gDev, devNum, 3);
    return 0;
}
static int __devexit
s3c2410led_remove(struct platform_device *pdev)
{
	cdev_del(gDev);
	unregister_chrdev_region(devNum, subDevNum);
}
static struct resource s3c_led_resource[] = {
	[0] = {
		.start = LEDBASE,
		.end   = LEDBASE + LEDLEN - 1,
		.flags = IORESOURCE_MEM,
	}
};
struct platform_device s3c_device_led = {
	.name		  = "s3c2410-led",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_led_resource),
	.resource	  = s3c_led_resource,
};
static struct platform_driver s3c2410led_driver = {
	.probe		= s3c2410led_probe,
	.remove		= __devexit_p(s3c2410led_remove),
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "s3c2410-led",
	},
};

int charDrvInit(void)
{
	platform_device_register(&s3c_device_led);
	//把平台设备结构体添加到链表里
    return platform_driver_register(&s3c2410led_driver); 
}

void __exit charDrvExit(void)
{
	platform_device_unregister(&s3c_device_led);
    platform_driver_unregister(&s3c2410led_driver);

    return;
}


module_init(charDrvInit);

module_exit(charDrvExit);

MODULE_LICENSE("GPL");

