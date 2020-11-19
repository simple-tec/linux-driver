#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>


#define DATA_NUM    (32)
int main(int argc, char *argv[])
{
    int fd, i;
    int r_len, w_len;
    fd_set fdset;
    char rbuf[DATA_NUM];
    memset(rbuf,0,DATA_NUM);
    fd = open("/dev/hello", O_RDWR);
    if(-1 == fd) {
      	perror("open file error\n");
		return -1;
    }	
	else {
		printf("open successe\n");
	}
    
	r_len = read(fd, rbuf, DATA_NUM);
	if(r_len == -1) {
		perror("read error\n");
		return -1;
	}
    printf("read len: %d\n", r_len);
    printf("%s\n", rbuf);
	close(fd);
    return 0;
}

