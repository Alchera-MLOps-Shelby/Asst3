#ifndef LIBNETFILES_H
#define LIBNETFILES_H

#define UNRESTRICTED_MODE 0
#define EXCLUSIVE_MODE 1
#define TRANSACTION_MODE 2
#define PORTNUM "23456"
#include <sys/types.h>
#include <sys/socket.h>

typedef struct packets
{
 char flagtype; // Type of flag "r" "w" "a" "e" read write all error
 int errOrFd; // Set to errno or Filedescriptor 
 size_t size;
} Packet;

typedef struct client
{
 int socket;
 struct sock_addr_in * address;
 socklen_t addr_size;  
} Client;
void *get_in_addr(struct sockaddr *sa);
int netopen(const char *pathname, int flags);
int netclose(int fd);
ssize_t netread(int fildes, char *buf, size_t nbyte);
ssize_t netwrite(int fildes, char *buf, size_t nbyte);
//int netserverinit(char * hostname, int filemode);
	//^Extension A; replace below
int netserverinit(char * hostname, int filemode);

#endif