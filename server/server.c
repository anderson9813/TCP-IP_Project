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

void sentFile(int sockfd, char *mystring)
{ 
	char buff[MAX]; //For read operation from file and used to sent operation 
 
	//Create file 
	FILE *fp;
 	fp=fopen(mystring,"r");  //Open file uses both stdio and stdin header files
	//File should be present at the program directory
	if( fp == NULL )
	{
 		printf("Error IN Opening File .. \n");
  		return ;
 	}
 
 	while ( fgets(buff,MAX,fp) != NULL ) //fgets reads upto MAX character or EOF 
  	write(sockfd,buff,sizeof(buff));  //Sent the file data to stream
 
 	fclose (fp);       //Close the file 
 
 	printf("File Sent successfully !!! \n"); 
}

void list(int sockfd)
{
	struct dirent *de;  //Pointer for directory entry 
  
    	//opendir() returns a pointer of DIR type.  
	

	char cwd[PATH_MAX];
  	if (getcwd(cwd, sizeof(cwd)) != NULL) 
	{
		
   		DIR *dr = opendir(cwd); 
  
    		if (dr == NULL)  //opendir returns NULL if couldn't open directory 
    		{ 
        		printf("Could not open current directory" ); 
    		}

		char buff1[MAX]; 

    		while ((de = readdir(dr)) != NULL)
		{
            		write(sockfd,de->d_name,sizeof(buff1));
  		}
    		closedir(dr);

	} 
	else 
	{
       		perror("getcwd() error");
   	}
   
}

void deletefile(int sockfd, char *mystring1) 
{
   	if (remove(mystring1) == 0)
	{
		char output[1000]= "1"; 
		printf("Deleted successfully \n");
		write(sockfd,output,sizeof(output));
	} 
   	else
	{
		char output[1000]= "2"; 
      		printf("Unable to delete the file \n");
		write(sockfd,output,sizeof(output));
	}
} 
