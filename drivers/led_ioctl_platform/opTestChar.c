/*****************************************************************************
¼ò    Êö£º¼òµ¥×Ö·ûÐÍÇý¶¯µÄ²âÊÔ³ÌÐò£¬²âÊÔled
******************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>


#define DATA_NUM      (10)

#define GETARG        (0)
#define SETARG        (1)

#define OK            (0)
#define ERROR         (-1)

#define LED_ENABLE    (0)
#define LED_DISABLE   (1)

int main(int argc, char *argv[])
{
    int fd, i;
    fd_set fdset;

    char buf[DATA_NUM], tmp[DATA_NUM];

    fd = open("/dev/testchar", O_RDWR);

    if(-1 == fd)
        {
        printf("open file error\r\n");
        }

    while(1)
        {
        if(0 == ioctl(fd, LED_ENABLE, 0))
		printf("led turn on\n");
        sleep(1);

        if(0 == ioctl(fd, LED_DISABLE, 0))
        	printf("led turn off\n");

        sleep(1);
        }
    
    return 0;
}

