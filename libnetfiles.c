#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "libnetfiles.h"

int main(int argc, char** argv) {
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
	return -1;
}










