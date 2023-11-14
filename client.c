#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define MAX 256

int main(int argc, char *argv[]){

    char letter;
    char *fn;
	int clientSocket, ret, n, found, l, tries;
	struct sockaddr_in serverAddr;
	char buff[MAX];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
	    printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

    n = write(clientSocket,"Hi ",strlen("Hi "));         //Welcoming the client
    if (n < 0)
        error("ERROR writing to socket");

    bzero(buff, sizeof(buff));
    read(clientSocket, buff, sizeof(buff));				//Read Server Welcoming
    printf("Server - %s\n", buff);

    n = write(clientSocket,"Send me the details of the game. ",strlen("Send me the details of the game. "));         //Welcoming the client
    if (n < 0)
        error("ERROR writing to socket");

    bzero(buff,sizeof(buff));
    read(clientSocket, &l, sizeof(int));           //NUmber of letters

    printf("The letters of the word are %d.\n", l);


    while(found !=1 && tries != 0) {

        bzero(buff, sizeof(buff));
        read(clientSocket, buff, sizeof(buff));  //Read the word and the missing letters from the server
        printf("%s\n", buff);

        write(clientSocket,"I received the word. Wait for my guess. ", strlen("I received the word. Wait for my guess. "));

        bzero(buff, sizeof(buff));
        read(clientSocket, buff, sizeof(buff));     //Enter a letter msg
        printf("%s\t", buff);

        scanf(" %c", &letter);
        write(clientSocket, &letter, sizeof(char));

        read(clientSocket, &found, sizeof(int));

        if(found == 1){
            printf("Server - You found the word!\n");

            write(clientSocket,"Thanks for the game. ",strlen("Thanks for the game. "));
        }
        else {
            read(clientSocket, &tries, sizeof(int));
            printf("Server - The tries you have left are %d\n", tries);
            write(clientSocket,"I'm ready for my next guess. ",strlen("I'm ready for my next guess. "));
        }

    }

    bzero(buff, sizeof(buff));
    read(clientSocket, buff, sizeof(buff));
    printf("%s\n", buff);

	close(clientSocket);
    return 0;
}
