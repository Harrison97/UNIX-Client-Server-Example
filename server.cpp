//Harrison Hayes
//Paris
//Operating Systems
//April 9th, 2018

/* A simple server in the internet domain using TCP
   The port nu1mber is passed as an argument */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <vector>
#include <fstream>
#define MAXHOSTNAME 256
#define MAXBUFFER 256
struct Major {
	char name[MAXBUFFER];
	int earlyPay, midPay;
	Major(){}//empty constructor
	Major(char n[MAXBUFFER], int e, int m) {
		strcpy(name, n);
		earlyPay = e;
		midPay = m;
	}
	void print() {
		std::cout << name << " " << earlyPay << " " 
		<< midPay << std::endl;
	}


};

void stobuf(std::string s, char b[MAXBUFFER], int maxbuf){
	for(int i = 0; i < MAXBUFFER; i ++) {
		if(s[i] = '\n') s[i] = '\0';
		if( i < s.length()) {
			b[i] = s[i];
		} else {
			b[i] = '\0';
		}
	}
}
void buftos(std::string &s, char b[MAXBUFFER], int maxbuf){
	for(int i = 0;i < MAXBUFFER; i ++) {
		if(b[i] == '\n') break;
		//if(b[i] = '\n') s[i] = '\0';
		//if( i < s.length()) {
			s[i] = b[i];
		//} else {
		//	b[i] = '\0';
		//}
	}
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

	 
	std::vector<Major> majors;
	std::string s;
	Major m;
	char myname[257];
	gethostname(myname, 257);
	std::cout << "Hostname : " << myname << std::endl;


	std::cout << "Enter file name : ";
	std::cin >> s;

	std::ifstream file(s);
	if(!file){
		std::cout << "error with file"<< std::endl;
		return -1;
	}

	std::cout << "Enter port : ";
	std::cin >> portno;



	//while loop parses the incoming file and fills our data
	while(getline(file, s)) {
		std::string name = "", numTemp = "";
		int earlyPay = -1, midPay = -1;

		//for loop loops the line and parses it for the correct variables
		for(int i = 0; i < s.length(); i++) {
			if(isalpha(s[i]) || ispunct(s[i]) ||  s[i] == ' ' &&
				earlyPay == -1) {
				name += s[i];
			} else if(isdigit(s[i])) {
				numTemp += s[i];
			} else if(!numTemp.empty()){
				if(earlyPay == -1) {
					earlyPay = stoi(numTemp);
					numTemp = "";//reset the temp after setting earlyPay
					name.pop_back();//erase last space from name
				} else {
					midPay = stoi(numTemp);
				}

			}
		}
		char temp[MAXBUFFER];
		strcpy(temp, name.c_str());
		m = Major(temp, earlyPay, midPay);
		majors.push_back(m);
		//m.print();

	}



     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        perror("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     //portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              perror("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     
	 while(true) {//loops for accepting new messages
		 newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
	     if (newsockfd < 0) 
	          perror("ERROR on accept");
	     bzero(buffer,256);
	     n = read(newsockfd,buffer,256);
	     if (n < 0) perror("ERROR reading from socket");
	     printf("Here is the message: %s",buffer);
///// my work starts (where i make changes to the example)


		for (int i = 0; i < MAXBUFFER; i ++ ) {
			//std::cout << i << ":" << buffer[i]<<std::endl;
		}
		std::string inmsg(buffer);
		for(int i = 0; i < MAXBUFFER; i++) {
			if (buffer[i] == '\n') buffer[i] = '\0';
		}
		std::string outmsg = "";

		//buftos(inmsg, buffer, MAXBUFFER);
	     //printf("Here is the message: %s\n",inmsg);
		buftos(inmsg, buffer, MAXBUFFER);
		//for loop creates the message
		for (int i = 0; i < majors.size(); i++) {
			//std::cout << buffer << "?" <<majors[i].name<<std::endl;
			if(strcmp(majors[i].name, buffer) == 0) {
				outmsg+="The early pay is ";
				outmsg+=std::to_string(majors[i].earlyPay);
				outmsg+=". The mid pay is  ";
				outmsg+=std::to_string(majors[i].midPay);
				outmsg+=".";
			}
		}
		char temp[MAXBUFFER];
		if(outmsg.empty()){
			if(buffer[0] == '\0') std::cout << "client disconnect" << std::endl;
			else {
				std::cout << "Not a major"<< std::endl;
				strcpy(temp, "not a major");
			}
		} else {
			std::cout << "Sending: " << outmsg << std::endl;
			strcpy(temp, outmsg.c_str());
		}


///// end of my work in the loop
	     //n = write(newsockfd,"I got your message",18);
	     n = write(newsockfd, temp, MAXBUFFER);
	     if (n < 0) perror("ERROR writing to socket");
     }
	 return 0; 
}
