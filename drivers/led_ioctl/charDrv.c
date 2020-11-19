/*****************************************************************************
简    述：简单字符型驱动程序，测试gpio
******************************************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <asm/io.h>

#define BUFFER_MAX    (10)

#define LED_ENABLE    (0)
#define LED_DISABLE   (1)

#define OK            (0)
#define ERROR         (-1)

#define GPBCON        (0x56000010)
#define GPBDAT        (0x56000014)
#define GPBUP         (0x56000018)

#define GPSIZE        (0x4)


struct cdev *gDev;
struct file_operations *gFile;
dev_t  devNum;
unsigned int subDevNum = 1;
 volatile unsigned long *data;

int reg_major  =  232;    

int reg_minor =   0;

unsigned long led_time = 1;/* 闪灯的时间间隔，最小为1秒(默认) */ 

int testOpen(struct inode *p, struct file *f)
{
    volatile unsigned long *tmp;
    //
    tmp = (volatile unsigned long *)ioremap(GPBUP,GPSIZE);
    *tmp = *tmp & (~(0x1<<5)) | (0x1<<5);//disable the pull up function
    iounmap(tmp);
    //
    
    tmp = (volatile unsigned long *)ioremap(GPBCON,GPSIZE);
    *tmp = *tmp & (~(0x1<<10)) | (0x1<<10);//set GPB5 to output
    iounmap(tmp);

    data = (volatile unsigned long *)ioremap(GPBDAT,GPSIZE);

    printk(KERN_EMERG"testOpen\r\n");
    
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
    switch(cmd)
        {
        case LED_ENABLE:
            *data = *data &(~(0x1<<5))|(0x1<<5);//set output to 1 
            break;
            
        case LED_DISABLE:
            *data = *data &(~(0x1<<5)) | (0x0<<5);//set output to 0
            break;
            
        default:
            printk(KERN_EMERG"testIoctl arg error\r\n");
        }

    return 0;
}
int testClose(struct inode *p, struct file *f)
{
	iounmap(data);
}

int charDrvInit(void)
{
    
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
    gFile->ioctl = testIoctl;
    gFile->release =  testClose;
    gDev->owner = THIS_MODULE;
    
    cdev_init(gDev, gFile);
    cdev_add(gDev, devNum, 3);

    return OK;
}

void __exit charDrvExit(void)
{

    cdev_del(gDev);
    unregister_chrdev_region(devNum, 1);

    return;
}


module_init(charDrvInit);

module_exit(charDrvExit);

MODULE_LICENSE("GPL");

