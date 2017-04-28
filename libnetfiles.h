#ifndef LIBNETFILES_H
#define LIBNETFILES_H

int netopen(const char *pathname, int flags);
int netclose(int fd);
ssize_t netread(int fildes, const void *buf, size_t nbyte);
ssize_t netwrite(int fildes, const void *buf, size_t nbyte);
//int netserverinit(char * hostname, int filemode);
	//^Extension A; replace below
int netserverinit(char * hostname);

#endif