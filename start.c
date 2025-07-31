#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define LISTEN_BACKLOG 50

void error(const char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(){

	//0 Für Protocl, das ist einfach deafult
	int serverSocket = socket(AF_INET, SOCK_STREAM,0);
	if(serverSocket == -1){
		error("Error creating the socket");
	}

	struct sockaddr_in serverSocketAdress;
	memset(&serverSocketAdress, 0, sizeof(serverSocketAdress));
	serverSocketAdress.sin_family = AF_INET;
	serverSocketAdress.sin_port = htons(12345);
	serverSocketAdress.sin_addr.s_addr = INADDR_ANY;
	

	if(bind(serverSocket,(struct sockaddr*) &serverSocketAdress ,sizeof(serverSocketAdress)) == -1){
		error("Binding the socket to an adress failed");
	}

	if(listen(serverSocket,LISTEN_BACKLOG) == -1){
		error("Error listening for a connection");
	}

	//ACCEPT ...

	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);

	int clientSocket = accept(serverSocket, (struct sockaddr*)& clientAddr, &clientLen);


	if(clientSocket == -1){
		error("Error accpeting the connection");
	}
	

	//if(inet_pton(AF_INET, "127.0.0.1", &socketAdress.sin_addr) <= 0){
    //    error("Invalid address/ Address not supported");
    //}
	
	//if(connect(serverSocket,(struct sockaddr*) &socketAdress, sizeof(socketAdress)) == -1){
	//	error("Error, conneting the socket with the server failed");
	//}

	close(serverSocket);
	

	return 0;
}
