#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "libnetfiles.h"

int global_flag = -1;
int first = 0;

 void call_open(int clientfd, char *buf)
 {
  //printf("in call_open\n");
  char pathname[700] = {0};
  Packet MessageRecv;
  

  errno = 0;
  strcpy(pathname, buf);
  memcpy(pathname, &buf[2], strlen(buf)-1);
  strcat(pathname, "\0");
  char mode = buf[1];
  //printf("mode: %c\n", mode);
  //printf("Pathname: %s\n",pathname);
  if(mode == 'r')
  {
   MessageRecv.errOrFd = open(pathname, O_RDONLY);
   global_flag = 0;
  }
  else if(mode == 'w')
  {
   MessageRecv.errOrFd = open(pathname, O_WRONLY);
   global_flag = 1;
  } 
  else if(mode == 'b')
  {
    MessageRecv.errOrFd = open(pathname, O_RDWR);
    global_flag = 2;
  }
  
  

  if(MessageRecv.errOrFd == -1)
  {
    perror("ERROR: Could not open file\n");
    MessageRecv.flagtype = 'e';
    MessageRecv.errOrFd = errno;
    send(clientfd, &MessageRecv, sizeof(MessageRecv), 0);
    return;
  }
  MessageRecv.flagtype = 'r';
  //MessageRecv.errOrFd = (MessageRecv.errOrFd * -1);
  //int negclFd = clientfd * -1;
  //printf("negclFd: %d\n",negclFd);
 //printf("opened file b4 send\n");
  send(clientfd, &MessageRecv, sizeof(MessageRecv), 0);
 // printf("after send\n");
  return;
 }


 void call_read(Client client, char *buf)
 {
   // printf("in call_read\n");
  
  Packet *MessageRecv;
  char *MessageSend;
  int clientfd = client.socket;
  int fd;

  size_t nbytes;
  ssize_t totalBytesRead;

  MessageRecv = (Packet*)buf;
  fd = MessageRecv->errOrFd; // Filedescriptor
  nbytes = MessageRecv->size;
  
  MessageSend = malloc(nbytes + 9);
  MessageSend[0] = 'r';
  errno = 0;
  totalBytesRead = read(fd, MessageSend + 9,nbytes);
  
  if(global_flag == 1)
  {
   Packet err;
   err.flagtype = 'e';
   err.errOrFd = errno;
   perror("ERROR: Permission denied-Write only\n");
   send(clientfd, &err, sizeof(err), 0);
   return;
  }
  if(totalBytesRead == -1)
  {
   Packet err;
   err.flagtype = 'e';
   err.errOrFd = errno;
   perror("ERROR: Could not read file\n");
   send(clientfd, &err, sizeof(err), 0);
  }

  else
  {
   printf("Read %ld bytes from file\n", totalBytesRead);
   memcpy(MessageSend + 1, &totalBytesRead, sizeof(totalBytesRead));
   send(clientfd, MessageSend, totalBytesRead + 9, 0);
   free(MessageSend);
  }
 }

void call_write(Client client, char *buf)
{
 //printf("in call_write\n");   
 
 Packet MessageSend;
 int fd;
 int clientfd = client.socket;
 size_t nbytes;
 ssize_t totalBytesWritten;

 memcpy(&fd, buf + 1, sizeof(int));
 memcpy(&nbytes, buf + 5, sizeof(size_t));

 errno = 0;
 totalBytesWritten = write(fd, buf + 13, nbytes);
 if(global_flag == 0)
 {
  perror("ERROR: Permission denied-READ ONLY\n");
  MessageSend.flagtype = 'e';
  MessageSend.errOrFd = errno;
  send(clientfd, &MessageSend,sizeof(MessageSend),0);
  return;
 } 
 
 if(totalBytesWritten == -1)
 {
  perror("ERROR: Could not write to file\n");
  MessageSend.flagtype = 'e';
  MessageSend.errOrFd = errno;
  send(clientfd, &MessageSend,sizeof(MessageSend),0);
  return;
 }

  printf("Wrote %ld bytes to file.\n", totalBytesWritten);
  MessageSend.flagtype = 'w';
  MessageSend.size = totalBytesWritten;
  send(clientfd,&MessageSend,sizeof(MessageSend),0);
 return;
}

void call_close(int clientfd, char * buf)
{
// printf("in call_close\n");
 Packet MessageSend;
 Packet *MessageRecv = (Packet*)buf;
 errno = 0;
 if(close(MessageRecv->errOrFd)==-1)
 {
  MessageSend.flagtype = 'e';
  MessageSend.errOrFd = errno;
  perror("ERROR: File not properly closed\n");
 }
  else
  {
   printf("File closed\n");
  }
  send(clientfd, &MessageSend, sizeof(MessageSend),0);
}

void * client_servSystem(void * ptr)
 {
  //printf("In thread client_servSystem!\n");
  Client client = *(Client *)ptr;
  int clientfd = client.socket;
  char buf[5000];

  while(1)
  {
   memset(buf, 0, sizeof(buf));
   int recval = recv(clientfd, buf, sizeof(buf), 0);

   if(recval == -1)
   {
    perror("ERROR: Could not receive\n");
    return NULL;
   }
   else if(recval == 0)
   {
    printf("Connection has been terminated\n");
    close(clientfd);
    return NULL;
   }
   else
   {
    char filemethod = buf[0];
    //printf("buf[0]: %c\n", buf[0]);
    //printf("buf[1]: %c\n", buf[1]);

    if(filemethod == 'o')
    {
     call_open(clientfd,buf);
    }
    else if(filemethod == 'r')
    {
     call_read(client, buf);
    }
    else if(filemethod == 'w')
    {
     call_write(client, buf);
    }
    else if(filemethod == 'c')
    {
     call_close(clientfd, buf);
    }
    else
    {
     printf("ERROR: Invalid File I/O method\n");
    }
   }
  }  
   return NULL;
 }

int main(int argc, char * argv[])
{
//printf("WELL HELLO THERE\n");
 int serverfd, clientfd;
 struct addrinfo hints, *servinfo, *p;
 int rv;
 
 // Initialize addressinfo struct to 0
 memset(&hints, 0, sizeof(hints));
 
 // Fill up hints arguments
 hints.ai_family = AF_INET; // Make IPv4
 hints.ai_socktype = SOCK_STREAM; // Make TCP/IP protocol
 hints.ai_flags = AI_PASSIVE; // Use my own IP
 
 // Get the address information corresponding to the parameters passed into hints
 // Store result in servinfo
 if((rv = getaddrinfo(NULL, PORTNUM, &hints, &servinfo)) !=0)
 {
  printf("getaddrinfo: %s\n", gai_strerror(rv));
  exit(1);
 }
 
 // traverse through server info a socket file descriptor
 // is successfully found and bounded, break out of the loop
 // Otherwise the bind was unsuccessful
 //printf("b4 for loop\n");
 for(p = servinfo; p != NULL; p = p->ai_next)
 {
  serverfd = socket(p->ai_family,p->ai_socktype, p->ai_protocol);

  if(serverfd == -1)
  {
    perror("ERROR: socket\n");
  }
  else if(bind(serverfd, p->ai_addr, p->ai_addrlen) == -1)
  {
   close(serverfd);
   perror("ERROR: Current bind failed\n");
  }
  else
  {
   break;
  }
 }
  
  // if p is NULL then no socket has been binded(bound?).
  if(p == NULL)
  {
   printf("ERROR: Failed to bind to any socket\n");
   exit(2);
  }
  
  // Free memory no longer being used
  freeaddrinfo(servinfo);
  
  //printf("About to listen\n");
  // Test to see if you can hear anything, if not you're deaf..
  // If you can then WHOOHOO! Now we just need to sit and wait!
  if(listen(serverfd, 10) < 0)
  {
   perror("ERROR: I cannot hear you...Listen failed\n");
   exit(0);
  }
  
  // Get the size of a socket address to use once we get a connection!
  socklen_t socklen = sizeof(struct sockaddr_in);
  //printf("Before while\n");
  while(1)
  {
    if(first == 0)
    printf("Waiting for a connection\n");
   first = -1;// to say waiting for a connection only first time
   Client *client = (Client *)malloc(sizeof(Client));
   client->address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
   clientfd = accept(serverfd, (struct sockaddr *)client->address, &socklen);
   
   // set the client objects socket to what listen accepted.
   client->socket = clientfd;
   client->addr_size = socklen;

   printf("Connection has been made to a client\n");

   pthread_t * thread =(pthread_t *)malloc(sizeof(pthread_t));
   pthread_create(thread, NULL, client_servSystem, (void*)client);    
   
  }
 }

 


