
#include <stdlib.h> //for basic memmory allocation and deallocation
#include <stdio.h> //for file read and write
#include <netdb.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "server.h"
#define MAX 100
#define PORT 8080 
#define SA struct sockaddr
#define FIFO_FILE_1  "/tmp/client_to_server_fifo"
#define FIFO_FILE_2  "/tmp/server_to_client_fifo"


int main() 
{
 
	int client_to_server;
    	int server_to_client;   	
	char buf[BUFSIZ];
	char buf1[1000];
	char buf2[1000];
	
    	/* create the FIFO (named pipe) */
    	mkfifo(FIFO_FILE_1, 0666);
    	mkfifo(FIFO_FILE_2, 0666);

	int sockfd, connfd, len;     //Create socket file descriptor 
 	struct sockaddr_in servaddr, cli;   //Create structure object of sockaddr_in for client and server// socket create and verification 
 	sockfd = socket(AF_INET, SOCK_STREAM, 0);    //Creating a TCP socket ( SOCK_STREAM )
 
 	if (sockfd == -1) 
	{ 
  	printf("socket creation failed...\n"); 
  	exit(0); 
 	} 
 	else
  	printf("Socket successfully created..\n"); 
 
 
 	bzero(&servaddr, sizeof(servaddr)); //assign IP, PORT 
 	servaddr.sin_family = AF_INET;     //Specifies the address family with IPv4 Protocol 
 	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //Binds to any address
 	servaddr.sin_port = htons(PORT);     //Binds to PORT specified //Binding newly created socket to given IP and verification 
 	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) 
	{ 
		printf("socket bind failed...\n"); 
  		exit(0); 
 	} 
 	else
  		printf("Socket successfully binded...\n"); //Now the server is ready to listen and verification 
 	
	if ((listen(sockfd, 5)) != 0) 
	{ 
  		printf("Listen failed...\n"); 
  		exit(0); 
 	} 
 	else
  		printf("Server listening...\n"); 
 
 	len = sizeof(cli); //It accepts the data packet from client and verification 
 	connfd = accept(sockfd, (SA*)&cli, &len);  //It accepts connection from socket
 
 	if (connfd < 0) 
	{ 
  		printf("server acccept failed...\n"); 
  		exit(0); 
 	} 
 	else
  		printf("server acccept the client...\n"); //The function for chatting between the client and the server 
	
	
	client_to_server = open(FIFO_FILE_1, O_RDONLY);
       	server_to_client = open(FIFO_FILE_2, O_WRONLY);

        read(client_to_server, buf, BUFSIZ);

	int temp;
	temp = *buf; //Storing the decimal value pointed in the variable temp
	//char 1 = 49, 2 = 50, 3 = 51
	
	printf("\n");

	if (temp == 49)
	{
		read(client_to_server, buf1, 1000);
		printf("File to be transfered : %s \n", buf1);
		
		sentFile(connfd,buf1); //Calling the function
	
	}
	
	
	if (temp == 50) //Function to display the content of the server directory
	{

		list(connfd); //Calling the function list to display all the files 
	}


	if (temp == 51)
	{
		read(client_to_server, buf2, 1000);
		printf("File to be deleted : %s \n", buf2);

		deletefile(connfd, buf2); //Calling the delete file function
		
	}
	

	memset(buf, 0, sizeof(buf));

	close(client_to_server);
    	close(server_to_client);

	unlink(FIFO_FILE_1);
   	unlink(FIFO_FILE_2);

	
 
}