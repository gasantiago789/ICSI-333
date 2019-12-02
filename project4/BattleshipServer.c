/* Project 4 template
 * Please check the comments and hints, and fill the codes.
 * You only need to write your code in this file, and you do not
 * need to write the server and client sides in two files.
 *
 * You also need to modify the code from project 3 accordingly to
 * make it adapted to the network communication.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define SIZE 10


// you can change the struct as you like
struct move
{
    char letter;
    int number;
    char state[20];
    char ship[20];
    struct move *next;
};

char ipAddress[200], port[200];
int ourSocket,listenSocket;
int sockfd, sockend;
struct sockaddr_in servaddr, cli;
int addrlen = sizeof(servaddr);

void generateShip(char** board, int size, char letter) {
	int noGood = 1;
	int orientation , row, col;
    int curRow, curCol;
	while (noGood) {
		orientation = random() %2;
		if (orientation == 0) { // Horizontal
			row = random() % 10;
			col = random() % (10 - size);
		} else {
			row = random() % (10 - size);
			col = random() % 10;
		}
		int noObstructions = 1;
		for (int i=0;i<size;i++) {
			curRow = row, curCol = col;
			if (orientation == 0) { // Horizontal
				curCol += i;
			} else {
				curRow += i;
			}
			if (board[curRow][curCol] != '-')
                noObstructions = 0;
		}
		if (noObstructions == 0)
            continue;
		noGood = 0;
	}
	printf ("Ship %c at %c%d-%c%d, orientation = %s\n",letter, row+65, col,curRow+65, curCol, orientation==0?"horizontal" : "vertical");
	for (int i=0;i<size;i++) {
		curCol = col, curRow = row;
		if (orientation == 0) { // Horizontal
			curCol += i;
		} else {
			curRow += i;
		}
		board[curRow][curCol] = letter;
	}
}


void createSendingSocket(struct move *ourMove) {
    /*write the code here, you can refer to the lab9 handout */
    char servMessage[265] = "You have connected to the server!\n";

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(sockfd, 5);

    int clientsock;

    clientsock = accept(sockfd,  NULL, NULL);

    send(clientsock, ourMove, sizeof(servMessage), 0);

    close(sockfd);

}


void createListenSocket()
{
    /*write the code here, you can refer to the lab9 handout */
    
}



char** initialization(){
	if (ipAddress[0] == 0){
        printf("create listen socket");
        /*add function call of create listen socket*/

    }
	else{
        printf("create sending socket");
        /*add function call of create sending socket*/

    }

	int i, j;
	char **board = (char**)malloc(sizeof(char*)*SIZE);
	for (i = 0; i < SIZE; i++){
		board[i] = (char*)malloc(sizeof(char)*SIZE);
	}
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			board[i][j] = '-';
		}
	}

	generateShip(board, 2,'D');
	generateShip(board, 3,'S');
	generateShip(board, 3,'C');
	generateShip(board, 4,'B');
	generateShip(board, 5,'R');
    return board;
}



void insert_move(struct move **head, struct move **tail,struct move *temp){
     if (*head == NULL){
        /* List is currently empty. */
        *head = *tail = temp;
    }
    else{
        (*tail)->next = temp;
        *tail = (*tail)->next;
    }
}

void update_state(char* state, char ** board, struct move** head,struct move** tail, struct move* temp){
	int row, i, j;
    char letter = temp->letter;
    int col = temp->number;
    row = letter % 65;
    if(board[row][col] == '-'){
        strcpy(state, "MISS");
        strcpy(temp->state, "MISS");
        strcpy(temp->ship, "  ");
    }
    else{
        strcpy(state, "HIT");
        strcpy(temp->state, "HIT!");
        switch (board[row][col]){
            case 'C':  strcpy(temp->ship, "Crusier"); break;
            case 'R':  strcpy(temp->ship, "Carrier"); break;
            case 'B':  strcpy(temp->ship, "Battleship"); break;
            case 'S':  strcpy(temp->ship, "Submarine"); break;
            case 'D':  strcpy(temp->ship, "Destroyer"); break;
        }
        board[row][col]='X';
    }

    insert_move(head,tail,temp);
    int counter = 0;
    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            if(board[i][j] == '-' || board[i][j] == 'X')
                counter += 1;
        }
    }
    if(counter == SIZE * SIZE)
        strcpy(state, "GAME OVER!");
}

struct move* accept_input(){
    char letter;
    int number;
    bool flag = true;
    do{
        printf("Enter a letter A-J and number 0-9 ex. B4 - enter Z0 to end\n");
        int size = scanf(" %c%d", &letter, &number);
        if(size != 2){
            printf("INVALID INPUT\n");
            continue;
        }
        letter = toupper(letter);
        if(letter == 'Z' && number == 0)
            break;
        if (letter < 65 || letter > 74)
            printf("INVALID INPUT\n");
        else if (number <0 || number >9)
            printf("INVALID INPUT\n");
        else
            flag = false;
	}while(flag);

     struct move *temp;
     temp = (struct move *)malloc(sizeof(struct move));
     temp->letter = letter;
     temp->number = number;
    return temp;
}

void display_state(char* state, char** board)
{
	int i, j;
    printf("**** %s ****\n\n", state);

	printf(" 0 1 2 3 4 5 6 7 8 9\n");
	for (i = 0; i < SIZE; i++){
        printf("%c ", 65+i);
		for (j = 0; j < SIZE; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}

int teardown(char ** board,struct move* head)
        {
	int i;
    struct move* temp;
	for(i = 0; i < SIZE; i++)
		free(board[i]);
	free(board);
	FILE *fptr;

    fptr = fopen("log.txt", "w");
     if (fptr == NULL)
     {
         exit(-1);
     }


     if (head==NULL)
     {
         printf("The list is empty");
     }
     else{
         while (head != NULL){
            fprintf(fptr, "Fired at %c%d %s %s \n", head->letter, head->number, head->state, head->ship);
            temp = head;
            head = head->next;
            free(temp);
         }
     }
     fclose(fptr);
	return 0;
}




void main(int argc, char **argv)
{
	if (argc != 3 && argc != 2) { printf ("usage: battleship [ipaddress] port\n"); return; }
	if (argc == 3) {
        // if there are two command line arguments, where
        // first is the ipaddress and
        // second is the port, then we initialize
        // the server side in initialization() function
		strcpy(ipAddress,argv[1]);
		strcpy(port,argv[2]);
	}
	else {
        // if there is only one command line argument,
        // then we initialize the client side in initialization
        // function
		memset(ipAddress,0,200);
		strcpy(port,argv[1]);
	}

	srand(time(NULL));
	char** board;
    char state[] = "GAME START";
    char flag[] = "GAME OVER!";

    struct move *head, *tail, *ourMove;
    head = tail = NULL;

    /*modify the initialization function */
	board = initialization();

	do{
		display_state(state, board);
		ourMove = accept_input();
        /*add code below to send our move to the other player*/
        createSendingSocket(ourMove);
        /*add code to receive the state of our move from the other player*/

        /*add code to store our moves (letter, number, and result) into linked list*/

		struct move theirMove;
        /*add code below to receive theirMove from the other player*/

        /*modify the update_state function to check theirMove is HIT or MISS
         * and send the state back to the other player */

	} while(strcmp(state, flag));
	teardown(board, head);
	return 0;
}