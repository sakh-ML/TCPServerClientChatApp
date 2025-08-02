#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
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
	int clientSocket = socket(AF_INET, SOCK_STREAM,0);
	if(clientSocket == -1){
		error("Error creating the socket");
	}

	struct sockaddr_in clientSocketAdress;
	memset(&clientSocketAdress, 0, sizeof(clientSocketAdress));
	clientSocketAdress.sin_family = AF_INET;
	clientSocketAdress.sin_port = htons(PORT);
	clientSocketAdress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(clientSocket,(struct sockaddr*) &clientSocketAdress, sizeof(clientSocketAdress)) == -1){
		error("Error conneting to server");
	}

    while(true){

        char message[MESSAGE_SIZE];

        printf("Write a message to the server: ");
        fgets(message, MESSAGE_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        if(send(clientSocket, message, strlen(message), 0) == -1){
            error("Error sending the data to the server");
        }

        char buffer[BUFFER_SIZE] = {0};

        //reciving data from the server 
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if(bytesReceived < 0){
            error("Error reciving data from server");
        }
        else{
            buffer[bytesReceived] = '\0';
            printf("Message from server: %s\n", buffer);
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

