#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define DATA_NUM    (32)
int main(int argc, char *argv[])
{
    int fd, i;
    int r_len, w_len;
    fd_set fdset;
    char buf[DATA_NUM] = "hello world";
    fd = open("/dev/hello", O_RDWR);
    if(-1 == fd) {
      	perror("open file error\n");
		return -1;
    }	
	else {
		printf("open success\n");
	}
    w_len = write(fd, buf, DATA_NUM);
    if(w_len == -1) {
		perror("write error\n");
		return -1;
	}
	sleep(5);
    printf("write len: %d\n", w_len);
	close(fd);
    return 0;
}

