#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "libnetfiles.h"

int socketfd;
extern int errno;

int main(int argc, char** argv) {
	//Check for correct number of arguments, i.e.: <hostname> <filepath>
	if(argc != 3) {
		perror("Incorrect number of arguments\n");
		exit(1);
		return -1;
	}
	//Initialize hostname and filepath with corresponding char value
	char* hostname = argv[1];
	char* filepath = argv[2];
	//Check for correct hostname
		//filemode needed for Extension A; **need to enable way to get filemode
	int checkHostname = netserverinit(hostname, filemode);
	if(checkHostname == -1) {
		perror("Incorrect hostname\n");
		exit(1);
		return -1;
	}
	//netopen filepath with O_RDWR flag
	int netfd = netopen(filepath, O_RDWR);
	printf("open: %i\n", errno);
	//netread
	char netreadMessage[256];
	netread(netfd, netreadMessage, 10);
	printf("read: %i\n", errno);
	//netwrite
	char* netwriteStartmessage = "write";
	printf("write: %i\n", errno);
	//netclose
	netclose(netfd);
	printf("close: %i\n", errno);
	return 0;
}

/* 
The argument flags must include one of the following access modes: O_RDONLY,
O_WRONLY, or O_RDWR. These request opening the file read-only, write-only, or
read/write, respectively.

netopen() returns the new file descriptor, or -1 in the caller's contenxt if an error occurred (in
which case, errno is set appropriately). In order to avoid error and disambiguate your file
descriptors from the system's, make your file descriptors negative (but not -1!).

*/
int netopen(const char *pathname, int flags) {
	//Check for correct argument flags
	if(flags != O_RDONLY && flags != O_WRONLY && flags != O_RDWR) {
		perror("Incorrect netopen flags\n");
		exit(1);
		return -1;
	}
}

int netclose(int fd) {
	return -1;
}

ssize_t netread(int fildes, const void *buf, size_t nbyte) {
	return -1;
}

ssize_t netwrite(int fildes, const void *buf, size_t nbyte) {
	return -1;
}

int netserverinit(char * hostname, int filemode) {
}










