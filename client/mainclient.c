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






int main() 
{ 

 	int sockfd, connfd; 
 	struct sockaddr_in servaddr, cli; //The socket create and verification 
 	sockfd = socket(AF_INET, SOCK_STREAM, 0); 

 	if (sockfd == -1) 
	{ 
  		printf("socket creation failed...\n"); 
  		exit(0); 
 	} 
 	else
  		printf("Socket successfully created..\n"); 
 
 	bzero(&servaddr, sizeof(servaddr)); //Assign IP, PORT 
 	servaddr.sin_family = AF_INET; 
 	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
 	servaddr.sin_port = htons(PORT); //It connects the client socket to the server socket 
 	
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
	{ 
  		printf("connection with the server failed...\n"); 
  		exit(0); 
 	} 
 	else
  		printf("connected to the server..\n"); //The function for sending File 
 
	system("clear");
    	int client_to_server;
    	int server_to_client;

    	char str[240];
	
	printf("\033[1;35m");
	printf(" \n");
	printf("Choose one among these options : \n");
	printf("1) Obtain a document which is available on the server \n");
	printf("2) Obtain the list of documents available on the server \n");
	printf("3) Delete a document from the list on the server \n");
	printf("Your choice : ");
	fgets(str,sizeof(str),stdin);
    	str[strlen(str)-1] = '\0';
	printf("\033[0m");
	
    	/* Write str to the FIFO */
    	client_to_server = open(FIFO_FILE_1, O_WRONLY);
    	server_to_client = open(FIFO_FILE_2, O_RDONLY);

	write(client_to_server, str, sizeof(str));
	
	char str2[240];

	int temp;
	temp = *str; //Storing the decimal value pointed in the variable temp
	// char 1 = 49, 2 = 50, 3 = 51

	if (temp == 49)
	{
		printf("\n");
		
		printf("Which document do you wish to obtain? : ");
		fgets(str2,sizeof(str2),stdin);
    		str2[strlen(str2)-1] = '\0';
	
		write(client_to_server, str2, sizeof(str2));
		
		recvFile(sockfd); 
 		close(sockfd); 
		
	}
	
	else if (temp == 50)
	{
                printf("List of files on Server");
		list(sockfd); //Calling the function to display the files
	}

	else if (temp == 51) //Delete file option
	{
		printf("\n");
		
		printf("Which document do you wish to delete? : ");
		fgets(str2,sizeof(str2),stdin);
    		str2[strlen(str2)-1] = '\0';
	
		write(client_to_server, str2, sizeof(str2));
		
		char bufff[1000];
		read(client_to_server, bufff, BUFSIZ);
		if (*bufff == 51)
			printf("Failed to delete file\n");
		else 
			printf("Deleted successfully\n");
	}

	else
	{
		printf("\n");		
		printf("\033[1;31m");
		printf("You have inserted an invalid value! \n");
		printf("\033[0m");
	} 	
	
	close(client_to_server);
    	close(server_to_client);

	
}