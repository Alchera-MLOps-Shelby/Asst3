#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>
#include <netdb.h.h> 
#include "libnetfiles.h"
#define PORTNUM 12345
#define BACKLOG 20


// Creates new socket, & binds, then open file given by pathname
//flags: O_RDONLY,O_WRONLY,O_RDWR
//Return: new file descriptor(!= -1), or -1 if error(throw errno) to caller
//errors-to-check: EACCES,EINTR,EISDIR,ENOENT,EROFS(ENFILE,EWOULDBLOCK,EPERM)

int netopen(const char *pathname, int flags)
{
 struct addrinfo hints, *servinfo, *p;
 int sockfd, rv;
 int yes = 1;
 //Check for correct argument flags
 if((flags != O_RDONLY) && (flags != O_WRONLY) && (flags != O_RDWR))
 {
  return -1;
 }
  
  memset(&hints, 0, sizeof(hints));//make sure the struct is empty
  hints.ai_family = AF_UNSPEC; // don't care whether IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP protocol
  hints.ai_flags = AI_PASSIVE; // fill in IP address automatically
  
  // MAKE SERVER INFO POINT TO A LINKED LIST OF 1 or MORE STRUCT ADDRINFOS
  if((rv = getaddrinfo(NULL, PORTNUM, &hints, &servinfo)) != 0)
  {
  	fprintf(stderr, "gettaddrinfo error: %s\n", gai+strerror(status));
  	return 1;
  }
  
  // loop through & bind to first socket possible
  for(p = servinfo; p != NULL; p = p->ai_next)
  { 
  	 //create socket file descriptor with parameters created from getaddrinfo() method
  	if((sockfd = socket(p->ai_family, p->ai_socktype)) == -1)
  	{
  	 ernno("server: socket");
  	 continue;
  	}
  

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==-1)
  {
  	errno("setsockopt");
  	return -1;
  }
  
  // Bind socket
  if(bind(sockfd, p_ai_addr, p->ai_addrlen) == -1)
  {
   close(sockfd);
   errno("Server: bind");
   continue;
  }
  break;
 }

   //sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
   //bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
  
  // open file
  sockfd = open(pathname, flags);
  
  freeaddrinfo(servinfo);
  
  return sockfd;
   




  // grab parameters
  // parse(pathname)
  // make message
  //write message 

  //read response
  //set retval &errno, h_errno
  //close socket
  //return
}

// Return: non-negative int indicating #bytes read, else return -1 & set errno in caller's context to indicate error
//Errors-to-check: ETIMEDOUT,EBADF,ECONNRESET
ssize_t netread(int fildes, void *buf, size_t nbyte)
{
 int k = 0;
 struct addrinfo hints, *servinfo, *p;
 int sockfd, rv;
 int yes = 1;
 //Check for correct argument flags
 if((flags != O_RDONLY) && (flags != O_WRONLY) && (flags != O_RDWR))
 {
  return -1;
 }
  
  memset(&hints, 0, sizeof(hints));//make sure the struct is empty
  hints.ai_family = AF_UNSPEC; // don't care whether IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP protocol
  hints.ai_flags = AI_PASSIVE; // fill in IP address automatically
  
  // MAKE SERVER INFO POINT TO A LINKED LIST OF 1 or MORE STRUCT ADDRINFOS
  if((rv = getaddrinfo(NULL, PORTNUM, &hints, &servinfo)) != 0)
  {
  	fprintf(stderr, "gettaddrinfo error: %s\n", gai+strerror(status));
  	return 1;
  }
  
  // loop through & bind to first socket possible
  for(p = servinfo; p != NULL; p = p->ai_next)
  { 
  	 //create socket file descriptor with parameters created from getaddrinfo() method
  	if((sockfd = socket(p->ai_family, p->ai_socktype)) == -1)
  	{
  	 ernno("server: socket");
  	 continue;
  	}
  

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==-1)
  {
  	errno("setsockopt");
  	return -1;
  }
  
  // Bind socket
  if(bind(sockfd, p_ai_addr, p->ai_addrlen) == -1)
  {
   close(sockfd);
   errno("Server: bind");
   continue;
  }
  break;
 }

 // WRITE READ CODE HERE with sockfd
 int numOfBytesRead;
 int totalBytesRead = 0;
 do
 {
  numOfBytesRead = read(fildes, buf, nbyte);
  totalBytesRead += numOfBytesRead;
 }while(numOfBytesRead != 0);
 
 if(totalBytesRead < 0)
 {
  return -1;
 }
 
 return totalBytesRead;
 
}

//Return: #bytes actually written to file assoc w/ fildes. never >nbyte, else -1 throw errno
//Errors-to-check: EBADF, ETIMEOUT,ECONNRESET
ssize_t netwrite(int fildes, const void *buf, size_t nbyte)
{
 struct addrinfo hints, *servinfo, *p;
 int sockfd, rv;
 int yes = 1;
 //Check for correct argument flags
 if((flags != O_RDONLY) && (flags != O_WRONLY) && (flags != O_RDWR))
 {
  return -1;
 }
  
  memset(&hints, 0, sizeof(hints));//make sure the struct is empty
  hints.ai_family = AF_UNSPEC; // don't care whether IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP protocol
  hints.ai_flags = AI_PASSIVE; // fill in IP address automatically
  
  // MAKE SERVER INFO POINT TO A LINKED LIST OF 1 or MORE STRUCT ADDRINFOS
  if((rv = getaddrinfo(NULL, PORTNUM, &hints, &servinfo)) != 0)
  {
  	fprintf(stderr, "gettaddrinfo error: %s\n", gai+strerror(status));
  	return 1;
  }
  
  // loop through & bind to first socket possible
  for(p = servinfo; p != NULL; p = p->ai_next)
  { 
  	 //create socket file descriptor with parameters created from getaddrinfo() method
  	if((sockfd = socket(p->ai_family, p->ai_socktype)) == -1)
  	{
  	 ernno("server: socket");
  	 continue;
  	}
  

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==-1)
  {
  	errno("setsockopt");
  	return -1;
  }
  
  // Bind socket
  if(bind(sockfd, p_ai_addr, p->ai_addrlen) == -1)
  {
   close(sockfd);
   errno("Server: bind");
   continue;
  }
  break;
 }
 
 // WRITE WRITE CODE HERE WITH sockfd
 int numbytesWritten;
 int totalBytesWritten = 0;
 do
 {
  numbytesWritten = write(fildes, buf, nbyte)
  if(numbytesWritten < 0)
  	break;
  totalBytesWritten += numbytesWritten;
 }while(numbytesWritten != 0);

 if(totalBytesWritten < 0)
 {	
  return -1;
 }
 return totalBytesWritten;
}

//Return: 0 on success, error -1 set errno
//Errors-to-check: EBADF
int netclose(int fd)
{
 close(fd);
}

//Return: 0 on success, error -1 & h_errnor set correctly***
// fct of type int?
//Errors-to-check: HOST_NOT_FOUND(ext A: INVALID_FILE_MODE)
int netserverinit(char * hostname) // ,int filemode) (ext A)
{
 struct sockaddr_in serverAddressInfo;
 struct hostent *serverIPAddress;
 
 memset((char*)&serverAddressInfo, 0, sizeof(serverAddressInfo));
 serverAddressInfo.sin_family = AF_INET;
 serverAddressInfo.sin_port = htons(PORTNUM);
 serverIPAddress = gethostbyname(hostname);
 
 if(serverIPAddress == NULL)
 {
  fprintf(stderr, "ERROR, no such host\n");
  return -1;
 }

 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if(sockfd < 0)
 {
  error("ERROR creating socket");
 }

 bzero((char *)&serverAddressInfo, sizeof(serverAddressInfo));

 serverAddressInfo.sin_family = AF_INET;
 serverAddressInfo.sin_port = htons(PORTNUM);

 bcopy((char*)serverIPAddress->h_addr,(char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);

 return 0;
}