/* Sample TCP client */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
int main(int argc, char**argv){
	int sockfd,n;
	struct sockaddr_in servaddr;
	char banner[] = "serverfile.tx";
	char buffer[1000];
	FILE *fp;
	char signs;
	int end=0,k=0;
	
	if (argc != 2){
		printf("usage: ./%s <IP address>\n",argv[0]);
		return -1;
	}

	/* socket to connect */
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	/* IP address information of the server to connect to */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port=htons(32000);
	
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	//RECIEVE data FROM server 
	sendto(sockfd,banner,strlen(banner),0, (struct sockaddr*)&servaddr,sizeof(servaddr));
	n=recvfrom(sockfd,buffer,1000,0,NULL,NULL);
	buffer[n]=0;
	printf("Received:%s\n",buffer);
	int i=0,j=0,count = 0;
	char size[10];
	
	//here get data size and how many times it need to wait is calculated
	if(buffer[0] == 'T'){
	   while(buffer[i]){
			if(isspace(buffer[i])) count++;
		//check data size part
		if(count == 6) { i++;
			while(1){
				size[j] = buffer[i];
				buffer[i] = '\0';
				i++;
				j++;
				if(!buffer[i]) break;
			}
			break;
		}
		
			buffer[i] = '\0';
			i++;
		}
	
	//this for MB and GB convention to byte with 1000 times
	signs = size[strlen(size)-2];
			
	switch(signs){
	
	case 'M' : for(j=0;j<strlen(size)-2;j++) {
			end = size[j]-48 + end*10;
			
		}
		end *= 1000;
		break;
		
	case 'G' : for(j=0;j<strlen(size)-2;j++) {
			end = size[j]-48 + end*10;
		}
		end *= 1000000;
		break;
	default : for(j=0;j<strlen(size)-1;j++) {
			end = size[j]-48 + end*10;
		}
		}
	
	//end for the break the loop..it stand for stop the loopaccording to 1000B set it get no of times
	end /= 1000;	
	end += 1;
	
	for(j=0;buffer[j];j++) buffer[j] = '\0';  //clear buffer
	
	fp = fopen("test.txt", "w+"); //open file for write data
	
	//stand in loop until recieve all data from server
	while(k < end){
		for(j=0;j<1000;j++) buffer[j] = '\0'; //clear data in array
		
		n=recvfrom(sockfd,buffer,1000,0,NULL,NULL); //recieve data from server
		printf("%s",buffer);	//print recieved data
		fputs(buffer,fp); 	//write to file
		k++;		
	}
	fclose(fp);
	//printf("\n");	
	
	}

return 0;
}
