#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libnetfiles.h"

// ./client <HOSTNAME> <FILEMODE> <FILENAME> <FLAGS>
int main(int argc, char *argv[])
{
 int filemode = atoi(argv[2]);
 int flags = atoi(argv[4]);
 printf("B4 init\n");
 netserverinit(argv[1],filemode);
 int fd;
 char buf[7000];
 printf("b4 open\n");
 fd = netopen(argv[3],flags);
 printf("b4 read\n");
 netread(fd,buf,20);
 printf("b4 write\n");
 netwrite(fd, buf, 20);
 printf("b4 close\n");
 netclose(fd);
 return 0;	
}