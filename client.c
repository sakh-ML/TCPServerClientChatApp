#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 9090
#define LISTEN_BACKLOG 50
#define BUFFER_SIZE 1024

void error(const char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(){

	//0 Für Protocl, das ist deafult protocol
	int clientSocket = socket(AF_INET, SOCK_STREAM,0);
	if(clientSocket == -1){
		error("Error creating the socket");
	}

	struct sockaddr_in clientSocketAdress;
	memset(&clientSocketAdress, 0, sizeof(clientSocketAdress));
	clientSocketAdress.sin_family = AF_INET;
	clientSocketAdress.sin_port = htons(PORT);
	clientSocketAdress.sin_addr.s_addr = INADDR_ANY;

    if(connect(clientSocket,(struct sockaddr*) &clientSocketAdress, sizeof(clientSocketAdress)) == -1){
		error("Error, conneting the socket with the server failed");
	}
	

	const char* message = "Heyy";

	if(send(clientSocket, message, strlen(message), 0) == -1){
		error("Error sending the data");
	}


	close(clientSocket);
	

	return 0;
}
