/*****************************************************************************
简    述：简单字符型驱动程序，采用阻塞方式
******************************************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>

#define BUFFER_MAX    (10)
#define OK            (0)
#define ERROR         (-1)

struct cdev *gDev;
struct file_operations *gFile;
dev_t  devNum;
unsigned int subDevNum = 1;
static wait_queue_head_t testqueue;

int reg_major  =  232;    

int reg_minor =   0;

char *buffer;


int flag = 0;


int testOpen(struct inode *p, struct file *f)
{
    printk(KERN_EMERG"testOpen\r\n");
    return 0;
}


ssize_t testWrite(struct file *f, const char __user *u, size_t s, loff_t *l)
{
    int writelen;
    printk(KERN_EMERG"begin testWrite\r\n");
    writelen = BUFFER_MAX>s ? s : BUFFER_MAX;
    copy_from_user(buffer, u, writelen);
    flag = 1;
    wake_up_interruptible(&testqueue);
    return writelen;

}

ssize_t testRead(struct file *f, char __user *u, size_t s, loff_t *l)
{
    int readlen;
    int ret;
    if(f->f_flags & O_NONBLOCK){//no block
	if(flag == 1){
		readlen = BUFFER_MAX>s ? s : BUFFER_MAX;
		copy_to_user(buffer, u, readlen);
		flag = 0;
		return readlen;
	}
	else{
		return -EAGAIN;
	}
		
    }	
    else{//block
    	  if(flag == 1){
		readlen = BUFFER_MAX>s ? s : BUFFER_MAX;
		copy_to_user(buffer, u, readlen);
		flag = 0;
		return readlen;
	 }
         else{//flag == 0
		ret = wait_event_interruptible(testqueue, flag);
		if(ret == 0){
			readlen = BUFFER_MAX>s ? s : BUFFER_MAX;
			copy_to_user(buffer, u, readlen);
			flag = 0;
			return readlen;
		}
		else{
			printk(KERN_INFO "interrupted by signal\n");
			return -ERESTARTSYS;
		}
	  }
    }
}



int charDrvInit(void)
{
    
    devNum = MKDEV(reg_major, reg_minor);

    printk(KERN_EMERG"devNum is %d\r\n", devNum);

    if(OK == register_chrdev_region(devNum, subDevNum, "testchar"))
        {
        printk(KERN_EMERG"register_chrdev_region ok\r\n");
        }
    else
        {
        printk(KERN_EMERG"register_chrdev_region error\r\n");
        return ERROR;
        }
        
	printk(KERN_EMERG"devNum is %d\r\n", devNum);
    gDev = kzalloc(sizeof(struct cdev), GFP_KERNEL);

    gFile = kzalloc(sizeof(struct file_operations), GFP_KERNEL);

    gFile->open = testOpen;
    gFile->read = testRead;
    gFile->write = testWrite;

    
    gFile->owner = THIS_MODULE;
    

    
    cdev_init(gDev, gFile);
    cdev_add(gDev, devNum, 3);
    init_waitqueue_head(&testqueue);

    buffer = kmalloc(BUFFER_MAX, GFP_KERNEL);
    if(NULL == buffer)
    {
        printk(KERN_EMERG" kmalloc error\r\n");
	return -1;
    }
    else
    {
        printk(KERN_EMERG" kmalloc successful\r\n");
        return 0;
    }
    return 0;
}

void __exit charDrvExit(void)
{
    kfree(buffer);
    cdev_del(gDev);
    unregister_chrdev_region(devNum, subDevNum);

    return;
}


module_init(charDrvInit);

module_exit(charDrvExit);

MODULE_LICENSE("GPL");

