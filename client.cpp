//Harrison Hayes
//Paris
//Operating Systems
//April 9th, 2018

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>

int main(int argc, char *argv[])
{

    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;
	char hostname[257];
    char buffer[256];
   // if (argc < 3) {
   //    fprintf(stderr,"usage %s hostname port\n", argv[0]);
   //    exit(0);
   // }
   std::cout << "Enter the hostname: ";
   std::cin >> hostname;
   std::cout << "Enter the port: ";
   std::cin >> portno;
   std::cin.ignore();
    //portno = atoi(argv[2]);
	while(true){
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0) 
	        printf("ERROR opening socket");
	    server = gethostbyname(hostname);
	    if (server == NULL) {
	        fprintf(stderr,"ERROR, no such host\n");
	        exit(0);
	    }
	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    bcopy((char *)server->h_addr, 
	         (char *)&serv_addr.sin_addr.s_addr,
	         server->h_length);
	    serv_addr.sin_port = htons(portno);
	    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
	        printf("ERROR connecting");
	    printf("Please enter the major: ");
	    bzero(buffer,256);
	    fgets(buffer,255,stdin);
		if (buffer[0] == '\n') break; //if empty string is entered
	    n = write(sockfd,buffer,strlen(buffer));
	    if (n < 0) 
	         printf("ERROR writing to socket");
	    bzero(buffer,256);
	    n = read(sockfd,buffer,255);
	    if (n < 0) 
	         printf("ERROR reading from socket");
	    printf("%s\n",buffer);
    
	}
	return 0;
}
