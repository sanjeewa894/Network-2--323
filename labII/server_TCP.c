/* Sample TCP server */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char**argv){

	int listenfd,connfd,n;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t clilen;
	char* banner;// = "The size of the file is 10Mb";
	char buffer[1000],temp[1000];
	
    FILE * sendfile;
    char c;
	/* one socket is dedicated to listening */
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	
	/* initialize a sockaddr_in struct with our own address information for binding the socket */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(32000);

	/* binding */
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	listen(listenfd,0);
	clilen=sizeof(cliaddr);
while(1){
	/* accept the client with a different socket. */
	connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen); // the uninitialized cliaddr variable is filled in with the

	n = recvfrom(connfd,buffer,1000,0,(struct sockaddr *)&cliaddr,&clilen);//information of the client by recvfrom function
	buffer[n] = 0;
	
	//read file from server
	if(buffer != NULL)
	    sendfile = fopen (buffer, "r");
	  
	//else banner = "No content of file specify";
	int i=0;
	
	//here send file to client 
	if(sendfile != NULL){
	  banner = "The size of the file is 1016b";    //the size of the file
	   sendto(connfd,banner,strlen(banner),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));  //send size
	   banner = NULL; 
	   
	   	int j;
	    while(1){		
	      	//file read part	
			c = fgetc(sendfile);
			temp[i] = c; 
			i++;			 
			
			if(feof(sendfile)) {
				banner = temp; 	
        		banner[strlen(banner)-1] = '\0'; //set last data to null
        		sendto(connfd,banner,strlen(banner),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr)); //send file data        			 
				break;
			}
	         	//chech fixed data size to send 
	        	if(i>=1000){
	        		banner = temp;
	        		i=0;
	        		sendto(connfd,banner,strlen(banner),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
				
				 for(j=0;j<1000;j++) temp[j] = '\0';	//clear data in array	
	        	}       
        }
            
       fclose(sendfile);
    } 
     //if file is not there..send message that there is no file like that
     else{
     	banner = "No content of file specify";
   	sendto(connfd,banner,strlen(banner),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr)); 
   	
   	}
   		
		printf("Received:%s\n",buffer); //print recieve content
	}
return 0;
}
