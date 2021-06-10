#include <netdb.h> 
#include <stdio.h>   
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "client.h"

#define MAX 100
#define PORT 8080 
#define SA struct sockaddr
#define FIFO_FILE_1  "/tmp/client_to_server_fifo"
#define FIFO_FILE_2  "/tmp/server_to_client_fifo"

 
void recvFile(int sockfd) 
{ 
 	char buff[MAX];  //To store message from client
 
 	FILE *fp;
 	fp=fopen("Received.txt","w"); //It stores the file content in Received.txt in the program directory
 
 	if( fp == NULL )
	{
  		printf("Error IN Opening File ");
  		return ;
 	}
 
 	while( read(sockfd,buff,MAX) > 0 )
  	fprintf(fp,"%s",buff);
 
 	printf("File received successfully !! \n");
 	printf("New File created is Received.txt !! \n");

}

void list(int sockfd)
{	
	char buff[MAX];
	while( read(sockfd,buff,MAX) > 0 )
  	printf("%s \n",buff);
}