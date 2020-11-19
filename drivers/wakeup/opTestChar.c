#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>


#define DATA_NUM    (10)

int main(int argc, char *argv[])
{
    	int fd, i;
    	fd_set fdset;

    	char buf[DATA_NUM], tmp[DATA_NUM];

   	 fd = open("/dev/testchar", O_RDWR);
	printf("%d\r\n",fd);
    	 if(-1 == fd)
        {
        	printf("open file error\r\n");
        }

//jinxin added to test	begin
	i=read(fd, tmp, DATA_NUM);
    	printf("read %d chars\r\n",i);
	return 0;
//jinxin added to test end

    	for(i = 0; i < DATA_NUM; i++)
        {
        	buf[i] = 'a';
        }
        printf("%s\r\n",buf);
        
     	i=write(fd, buf, DATA_NUM);
     	printf("write %d chars\r\n",i);
    	i=read(fd, tmp, DATA_NUM);
    	printf("read %d chars\r\n",i);
    	for(i = 0; i < DATA_NUM; i++)
        {
        	printf("%c\r\n",tmp[i]);
        }
    	

    	return 0;
}

