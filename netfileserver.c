#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include "libnetfiles.h"

struct buffStruct {
	char* buffer;
	int newSockfd;
}

void* handleClient(void* buff) {
	free(buffer);
	free(buff);
	return NULL;
}

int main(int argc, char** argv) {
	struct sockaddr_in serverAddressInfo;
	struct sockaddr_in clientAddressInfo;	
	struct buffStruct** buff;					//char array
	int portnum = 12345							//server port number
	int sockfd = -1;							//server socket file descriptor
	int bindCheck = -1;							//server socket binding check integer
	int clientAddressInfoSize = -1;				//clientAddressInfo struct size
	//Creates a socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//If socket creation unsuccessful, print error
	if(sockfd == -1) {
		perror("Socket creation unsuccessful");
	}
	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	//Empty the socket address info struct
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
	//Set remote port
	serverAddressInfo.sin_port = htons(portnum);
	//Set flag AF_INET (IPv4)
	serverAddressInfo.sin_family = AF_INET;
	//Set flag INADDR_ANY 
	serverAddressInfo.sin_addr.s_addr = INADDR_ANY;
	//Bind socket to port
	bindCheck = bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo));
	if (bindCheck == -1) {
		perror("Server socket bind to port unsuccessful");
	}
	//Listen for client connection(s)
	listen(sockfd, 20);
	//Initialize size of clientAddressInfo struct
	clientAddressInfoSize = sizeof(clientAddressInfo);
	//Allocate memory for buff
	buff = malloc(sizeof(struct buffStruct*));
	int buffIndex;
	//Main accept() loop
	while(1) {
		//Re-allocate memory for buff
		buff = realloc(buff, (buffIndex+1)*sizeof(struct buffStruct*));
		//Check if buff is NULL; print error if needed
		if(buff == NULL) {
			perror("realloc unsuccessful");
		}
		//Allocate memory for specific index in buff
		buff[buffIndex] = malloc(sizeof(struct buffStruct));
		//Allocate memory for char array
		buff[buffIndex] -> buffer = malloc(256);
		//Create a new client socket when client connects
		buff[buffIndex] -> newSockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, (unsigned int*) &clientAddressInfoSize); 

		//Client connection****

		//If client connection unsuccessful, print error
		if(buff[buffIndex] -> newSockfd == -1) {
			perror("New client socket creation unsuccessful");
		}
		//POSIX thread creation
		pthread_t pt;
		pthread_create(&pt, NULL, handleClient, buff[buffIndex]);
		//Increase buffIndex for new client
		buffIndex++;
	}
	return 0;
}