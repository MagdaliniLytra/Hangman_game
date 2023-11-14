#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define MAX_STRING_SIZE 50
#define MAX 256


int main(int argc, char *argv[]){
    char word[MAX_STRING_SIZE];
    char letter;
    int sock, ret, letters, i, l, n, found;
    int correct = 0, tries = 10;
    struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buff[MAX];
	pid_t childpid;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
        printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sock, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}

	newSocket = accept(sock, (struct sockaddr*)&newAddr, &addr_size);
	if(newSocket < 0){
		exit(1);
	}
	printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

    char str[MAX_STRING_SIZE], s[MAX_STRING_SIZE];

    FILE *fp;
    int readCount = 0;

    srand(time(NULL));
    fp = fopen("dictionary.txt", "r");

    if(fp) {
        if (fgets(s, MAX_STRING_SIZE, fp) != NULL) {
            readCount = 1;
            while(fgets(str, MAX_STRING_SIZE, fp) != NULL) {
                if ( (rand() % ++readCount)==0) {
                    strcpy(s, str);
                } // printf("%d\t%s", readCount, str);
            }
        }
    }

    fclose(fp);

    if (s[strlen(s)-1] == '\n') {
        s[strlen(s)-1] = '\0';
    }
    printf("The random word is %s but don't tell the client.", s);

    l = strlen(s);

    printf("The word has %d letters.\n", l);

    bzero(buff, sizeof(buff));
    read(newSocket, &buff, sizeof(buff));
    printf("Client - %s\n", buff);

    n = write(newSocket,"Welcome, let's start the game. Shall we? ",strlen("Welcome, let's start the game. Shall we? "));         //Welcoming the client
    if (n < 0)
        error("ERROR writing to socket");

    bzero(buff, sizeof(buff));
    read(newSocket, &buff, sizeof(buff));
    printf("Client - %s\n", buff);


    write(newSocket , &l , sizeof(int));

    switch(l) {
        case 3:
            word[0] = s[0];
            word[1] = '_';
            word[2] = s[l-1];
            break;
        case 4:
            word[0] = s[0];
            for(i=1 ; i<3 ; i++){
                word[i] = '_';
            }
            word[3] = s[l-1];
            break;
        case 5:
            word[0] = s[0];
            for(i=1 ; i<4 ; i++){
                word[i] = '_';
            }
            word[4] = s[l-1];
            break;
        case 6:
            word[0] = s[0];
            for(i=1 ; i<5 ; i++){
                word[i] = '_';
            }
            word[5] = s[l-1];
            break;
        case 7:
            word[0] = s[0];
            for(i=1 ; i<6 ; i++){
                word[i] = '_';
            }
            word[6] = s[l-1];
            break;
        case 8:
            word[0] = s[0];
            for(i=1 ; i<7 ; i++){
                word[i] = '_';
            }
            word[7] = s[l-1];
            break;
        case 9:
            word[0] = s[0];
            for(i=1 ; i<8 ; i++){
                word[i] = '_';
            }
            word[8] = s[l-1];
            break;
        case 10:
            word[0] = s[0];
            for(i=1 ; i<9 ; i++){
                word[i] = '_';
            }
            word[9] = s[l-1];
            break;
        case 11:
            word[0] = s[0];
            for(i=1 ; i<10 ; i++){
                word[i] = '_';
            }
            word[10] = s[l-1];
            break;
        case 12:
            word[0] = s[0];
            for(i=1 ; i<11 ; i++) {
                word[i] = '_';
            }
            word[11] = s[l-1];
            break;
        case 13:
            word[0] = s[0];
            for(i=1 ; i<12 ; i++) {
                word[i] = '_';
            }
            word[12] = s[l-1];
            break;
    }
    word[l] = '\0';

    while (found != 1 && tries != 0) {
        write(newSocket, word, strlen(word));

        bzero(buff, sizeof(buff));
        read(newSocket, &buff, sizeof(buff));
        printf("Client - %s\n", buff);

        write(newSocket, "Enter a letter: ", strlen("Enter a letter: "));

        bzero(buff, sizeof(buff));
        read(newSocket, &buff, sizeof(buff));
        printf("Client - The letter I chose is %s\n", buff);


        for (i = 1; i < l; i++) {
            if (*buff == s[i]) {
                word[i] = *buff;
                correct = 1;
            }
        }

        if (correct == 0) {
            tries -= 1;
        }
        correct = 0;

        write(newSocket, &found, sizeof(int));

        if (strcmp(word, s) == 0) {
            found = 1;
            printf("The client found the word!\n");
        }
        else {
            write(newSocket, &tries, sizeof(int));

            bzero(buff, sizeof(buff));
            read(newSocket, &buff, sizeof(buff));
            printf("Client - %s\n", buff);
        }
    }

    if(found == 1)
        write(newSocket, "||  You won, CONGRATULATIONS! ", strlen("You won, CONGRATULATIONS!! "));
    else
        write(newSocket, "You have lost. ", strlen("You have lost. "));

    close(newSocket);
    return 0;
}
