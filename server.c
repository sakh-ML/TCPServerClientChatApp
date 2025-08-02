#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h> 
#include <ctype.h>

#define PORT 9090
#define LISTEN_BACKLOG 50
#define BUFFER_SIZE 1024
#define MESSAGE_SIZE 1024

void error(const char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(){

	//0 Für Protocl, das ist deafult protocol
	int serverSocket = socket(AF_INET, SOCK_STREAM,0);
	if(serverSocket == -1){
		error("Error creating the socket");
	}

	struct sockaddr_in serverSocketAdress;
	memset(&serverSocketAdress, 0, sizeof(serverSocketAdress));
	serverSocketAdress.sin_family = AF_INET;
	serverSocketAdress.sin_port = htons(PORT);
	serverSocketAdress.sin_addr.s_addr = INADDR_ANY;
	

	if(bind(serverSocket,(struct sockaddr*) &serverSocketAdress ,sizeof(serverSocketAdress)) == -1){
		error("Binding the socket to an adress failed");
	}

	if(listen(serverSocket,LISTEN_BACKLOG) == -1){
		error("Error listening for a connection");
	}

	struct sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);

	int clientSocket = accept(serverSocket, (struct sockaddr*)& clientAddr, &clientLen);


	if(clientSocket == -1){
		error("Error accpeting the connection");
	}

	while (true){

		char buffer[BUFFER_SIZE] = {0};

		ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

		if(bytesReceived< 0){
			error("Error reciving the data from the client");
		}
		else if(bytesReceived == 0){
			error("Client Disconneted");
		}
		else{
			buffer[bytesReceived] = '\0';
			printf("Received from client: %s\n", buffer);
		}

		char message[MESSAGE_SIZE];
		printf("Write a message to the cleint: ");
		fgets(message, MESSAGE_SIZE, stdin);
		message[strcspn(message, "\n")] = '\0';

		if(send(clientSocket, message, strlen(message), 0) == -1){
			error("Error sending the data to the client");
		}

		char input[3];
		printf("Do you wanna send another message? Y or N: ");
		fgets(input, sizeof(input), stdin);
		if (tolower(input[0]) == 'n') {
			close(clientSocket);
			break;
		}
		else{
			continue;
		}
	}

	return 0;
}
