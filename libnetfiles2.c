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
#include <netdb.h> 
#include "libnetfiles.h"
#define BACKLOG 20

int serv_sock = -1;
int file_mode = -1;

// Creates new socket, & binds, then open file given by pathname
//flags: O_RDONLY,O_WRONLY,O_RDWR
//Return: new file descriptor(!= -1), or -1 if error(throw errno) to caller
//errors-to-check: EACCES,EINTR,EISDIR,ENOENT,EROFS(ENFILE,EWOULDBLOCK,EPERM)

int netopen(const char *pathname, int flags)
{
  Packets pack;
  // Check to make sure netserverinit has been called to handle socket binding
  if(serv_sock == -1)
  {
   perror("ERROR: Cannot OPEN file; Server socket connection not established\n");
   return -1;
  }
 char buffer[700];
 strcpy(buffer,"o");    
 //Check for correct argument flags
 if((flags != O_RDONLY) && (flags != O_WRONLY) && (flags != O_RDWR))
 {
  perror("ERROR: Flag of Invalid Type\n");
  return -1;
 }

 strcat(buffer, pathname);
 strcat(buffer, "\0");
   
 send(serv_sock, buffer, strlen(buffer), flags);
 recv(serv_sock, &pack, sizof(pack), flags);

 if(pack.flagtype == 'e')
 {
  errno = pack.errOrFd; // Returns errno
  perror("ERROR\n");
  return -1;
 }

 else if(pack.flagtype == 'r')
 {
  return pack.errOrFd; // Returns Filedescriptor
 }
  else
  return -1;
}

// Return: non-negative int indicating #bytes read, else return -1 & set errno in caller's context to indicate error
//Errors-to-check: ETIMEDOUT,EBADF,ECONNRESET
ssize_t netread(int fildes, void *buf, size_t nbyte)
{
  // Check to make sure netserverinit has been called to handle socket binding
  if(serv_sock == -1)
  {
   perror("ERROR: Cannot OPEN file; Server socket connection not established\n");
   return -1;
  }
  
  ssize_t totalBytesRead; 
  Packet messageSend;
  char* messageRecv = malloc(nbyte + sizeof(char) + sizeof(ssize_t));
  messageSend.flagtype = 'r';
  messageSend.errOrFd = fildes;
  messageSend.size = nbyte;
  
  send(server_sock, &messageSend, sizeof(messageSend), 0);
  recv(serv_sock, messageRecv, sizeof(messageRecv), 0);

  if(messageRecv[0] == 'e')
  {
    free(messageRecv);
    return -1;
  }
  else if(messageRecv[0] == 'r')
  {
   memcpy(&totalBytesRead, messageRecv + 1, sizeof(ssize_t));
   memcpy(buf, messageRecv + 9, totalBytesRead);

   free(messageRecv);
   return totalBytesRead;
  }
  return -1;
}

//Return: #bytes actually written to file assoc w/ fildes. never >nbyte, else -1 throw errno
//Errors-to-check: EBADF, ETIMEOUT,ECONNRESET
ssize_t netwrite(int fildes, const void *buf, size_t nbyte)
{
 // Check to make sure netserverinit has been called to handle socket binding
  if(serv_sock == -1)
  {
   perror("ERROR: Cannot OPEN file; Server socket connection not established\n");
   return -1;
  }
  
  char* messageSend = malloc(nbyte + 1 + sizeof(int) + sizeof(size_t));
  Packet messageRecv;

  messageSend[0] = 'w';
  memcpy(messageSend + 1, &filedes, sizeof(int));
  memcpy(messageSend + 1 + sizeof(int), &nbyte, sizeof(nbyte));
  memcpy(messageSend + 1 + sizeof(int) + sizeof(size_t),buf, nbyte);

  send(serv_sock, messageSend, sizeof(messageSend), 0);
  free(messageSend);
  recv(serv_sock, &messageRecv, sizeof(messageRecv),0);

  if(messageRecv.flagtype = 'e')
  {
   errno = messageRecv.errOrFd;
   return -1;
  }
  retrun messageRecv.size;
}

//Return: 0 on success, error -1 set errno
//Errors-to-check: EBADF
int netclose(int fd)
{
 // Check to make sure netserverinit has been called to handle socket binding
  if(serv_sock == -1)
  {
   perror("ERROR: Cannot OPEN file; Server socket connection not established\n");
   return -1;
  }
  
  Packet messageSend, messageRecv;
  messageSend.flagtype = 'c';
  message.errOrFd = fd; // set to filedescriptor

  send(serv_sock, &messageSend, sizeof(messageSend), 0);
  recv(serv_sock, &messageRecv, sizeof(messageRecv), 0);

  if(messageRecv.flagtype = 'e')
  {
   errno = messageRecv.errOrFd; // set to errno
   perror("ERROR: could not close properly\n");
  }

  return 0; 
}

//Return: 0 on success, error -1 & h_errnor set correctly***
// fct of type int?
//Errors-to-check: HOST_NOT_FOUND(ext A: INVALID_FILE_MODE)
int netserverinit(char * hostname, int filemode) {

    // Declare socket address struct
    struct addrinfo hints, *servinfo, *p;
    char s[INET6_ADDRSTRLEN]; // 46
    int sockfd, flag;

    // Fill in struct with zeroes
  bzero((char *)&hints, sizeof(hints));

  // Manually initialize the address information
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // Sets as TCP

    // Automatically initialize the address information from host
    if ((flag = getaddrinfo(hostname, PORTNUM, &hints, &servinfo)) != 0) 
    {
        fprintf(stderr, "Client: %s\n", gai_strerror(flag));
        return -1;
    }

    // Loop through server information for the appropriate address information to start a socket
  for (p = servinfo; p != NULL; p = p->ai_next) 
  {

    // Attempt to open socket with address information
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      perror("Client");
      continue;
    }

    // Connect to the server
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) {

      // Close socket if connection failed
      close(sockfd);
      continue;
    }

    // Successful connection found
    break;
  }

  // Check if socket was not bound
  if (p == NULL) 
  {
    return -1;
  }

  // Get IP address from socket address
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));

  // Free server information
  freeaddrinfo(servinfo);
  //set server socket filedescriptor to to the connection that was just made
  serv_sock = sockfd
  //set file_mode to the mode that was passed in through client
  file_mode = filemode;
  return 0;
}