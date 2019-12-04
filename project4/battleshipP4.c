/* CSI333. Programming at the Hardware-Software Interface
 * SEMESTER: FALL 2019
 * Lab Session: FRI 1:40PM
 * Name: Gabriel Santiago & Mike Scoopo
 * UA ID: 001316068 (Gabriel) & 001336856 (Mike)
 * Description:
 *      Program runs both Server and Client.
 *
 *      Program needs to be ran with your computer local IP Address (NOT LOCALHOST ADDR)
 *      Example: On windows run ipconfig and choose the IP address associated with your network
 *      On mac or linux use 'ifconfig getifaddr en0'
 *
 *      Use that IP Address.
 *
 *      When running, ./ProgramName [PORT] [IP ADDRESS]
 *
 *      Will load battleship game on server and client play and sends coordinates to attack. Client will receive
 *      state and which boat they hit.
 *
 *      Email me cwang25@albany.edu if you have any questions.
 */

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
#include<unistd.h>
#include <arpa/inet.h>


#define SIZE 10


//  Global variables that will be used
int connection;
int isServer;
char **board;
int counter;


struct move *head, **headPtr;

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

struct sockaddr_in servaddr, cli;

//  Prototypes
void update_state(char input[256]);
char *checkResult(char input[3]);
char *accept_input();


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

//  Creates the client
int createClient(char port[6], char ipaddres[17])
{
    int portnum = atoi(port);

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(portnum);
    //servaddr.sin_addr.s_addr = ipaddres;

    if(inet_pton(AF_INET, ipAddress, &servaddr.sin_addr) <= 0)
    {
        perror("Error. Failed to set address");
    }

    int connectionStat = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    if (connectionStat == -1)
    {
        printf("Error connecting to remote socket.\n");
    }

    isServer = 0;

    return sockfd;
}

//  These are the client calls
void clientUpDown(int connection)
{
    int isSending = 1; //   Checks to see if sending or receiving if 0 then receive, 1 for send

    char output[3];

    char input[256];

    while(true)
    {
        if (isSending)
        {
            //printf("Enter message to send to server\n");
            //fgets(message, sizeof(message), stdin);

            //write(connection itself server or client, variable, size of variable
            //  you can use this instead of send()
            strcpy(output, accept_input());

            write(connection, output, sizeof(output));  //  Sends data to connection

            recv(connection, input, sizeof(input), MSG_WAITALL); // Receives data

            update_state(input);
            /*len = strlen(message);
            if(message[len - 1] == "\n")
                message[len - 1] = '\0';

            if (strcmp(message, "exit") == 0)
            {
                break;
            }
            */
            isSending = 0;

        }
        else // Set client to receiving mode
        {
            recv(connection, input, sizeof(input), MSG_WAITALL); // Receives data
            /*printf("%s\n", input);
            len = strlen(message);
            if(message[len - 1] == "\n") // Replace \n with \0 (string corrector)
                message[len - 1] = '\0';
            if (strcmp(message, "exit") == 0)
            {
                break;
            }*/

            strcpy(output, checkResult(input));

            write(connection, output, sizeof(output));



            isSending = 1;
        }
        //break;
    }

}

//  Same as the client call but severside
void serverUpDown(int connection)
{
    int isSending = 0; //   Checks to see if sending or receiving if 0 then receive, 1 for send

    char output[3];

    char input[256];

    while(true)
    {
        if (isSending)
        {
            //printf("Enter message to send to server\n");
            //fgets(message, sizeof(message), stdin);

            //write(connection itself server or client, variable, size of variable
            //  you can use this instead of send()
            strcpy(output, accept_input());

            write(connection, output, sizeof(output));

            recv(connection, input, sizeof(input), 0);

            update_state(input);
            /*len = strlen(message);
            if(message[len - 1] == "\n")
                message[len - 1] = '\0';

            if (strcmp(message, "exit") == 0)
            {
                break;
            }
            */
            isSending = 0;

        }
        else
        {
            recv(connection, input, sizeof(input), 0);
            /*printf("%s\n", input);
            len = strlen(message);
            if(message[len - 1] == "\n") // Replace \n with \0 (string corrector)
                message[len - 1] = '\0';
            if (strcmp(message, "exit") == 0)
            {
                break;
            }*/

            strcpy(output, checkResult(input));

            write(connection, output, sizeof(output));



            isSending = 1;
        }
        //break;
    }
}

//  Creates the server itself
int createServer(char port[6])
{

    int portnum = atoi(port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("Sock is less than 0!\n");

    struct sockaddr_in servaddr;

    int addrlen = sizeof(servaddr);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(portnum);


    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        printf("Bind is less than 0!\n");


    if (listen(sockfd, 5) < 0)
        printf("Listen is less than 0!\n");

    int connection = accept(sockfd, (struct sockaddr *) &servaddr,(socklen_t*) &addrlen);

    if (connection < 0)
    {
        printf("Failed! Unable to accept connection. \n");
        exit(1);
    }
    isServer = 1;
    return connection;
}




char** initialization(int argc, char** argv)
{
    if (argc == 2)
    {

        connection = createServer(argv[1]);
        printf("Trying to create a server!\n");
    }
    else if (argc == 3)
    {

        connection = createClient(argv[1], argv[2]);
        printf("Trying to connect to server...\n");
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


int empty(struct move *temp) // Empties the node
{
    int k = 0;
    if (temp == NULL)
        k = 1;
    return k;
}

void insert_move(struct move *temp){
     /*if (*head == NULL){
        *//* List is currently empty. *//*
        *head = *tail = temp;
    }
    else{
        (*tail)->next = temp;
        *tail = (*tail)->next;
    }*/

    if(empty(*headPtr))
    {
        head = temp;
    }
    else
    {
        struct move **tempPtr;
        tempPtr = headPtr;
        while((*tempPtr)->next != NULL)
        {
            tempPtr = &(*tempPtr)->next;
        }

        (*tempPtr)->next = temp;


    }

}

char *checkResult(char input[3])
{
    //  Sets them back to int
    int row = input[0]-'A';
    int col = input[1]-'0';

    //char state[5];
    char ship[12];
    static char result[256];
    char flag;

    if (board[row][col] == '-') {
        //strcpy(state, "MISS");
        flag = '0';

    } else {
        //strcpy(state, "HIT");
        flag = '1';
        switch (board[row][col]) {
            case 'C':
                strcpy(ship, "Crusier");
                break;
            case 'R':
                strcpy(ship, "Carrier");
                break;
            case 'B':
                strcpy(ship, "Battleship");
                break;
            case 'S':
                strcpy(ship, "Submarine");
                break;
            case 'D':
                strcpy(ship, "Destroyer");
                break;
        }

    }

    snprintf(result, sizeof(result), "%s%s%s", &flag, input, ship);

    return result;
}

void update_state(char input[256]){
    char row, i, j, col;
    struct move temp ;

    //char letter = temp->letter;
    //int col = temp->number;
    //row = letter % 65;


    /*if(board[row][col] == '-'){
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
    }*/

    temp.letter = input[1];
    temp.number = input[2];

    if(input[0] == '0')
    {
        strcpy(temp.state, "Miss!");
        strcpy(temp.ship, "");

    } else if(input[0] == '1')
    {
        strcpy(temp.state, "Hit!");
        counter++;
        char *sub = &input[3];
        strcpy(temp.ship, sub);
    }

    board[row-'A'][col-'0']='X';

    insert_move(&temp);
    //int counter = 0;
    /*for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            if(board[i][j] == '-' || board[i][j] == 'X')
                counter += 1;
        }
    }
    if(counter == SIZE * SIZE)
        strcpy(state, "GAME OVER!");*/
}

char *accept_input()
{
    char letter;
    static char output[3];
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

     /*struct move *temp;
     temp = (struct move *)malloc(sizeof(struct move));
     temp->letter = letter;
     temp->number = number;
    return temp;*/
     output[0] = letter;
     output[1] = number+'0';

     return output;
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

int teardown()
        {
    int i;

    struct move *temp;
    for(i = 0; i < SIZE; i++)
        free(board[i]);
    free(board);
    FILE *fptr;

    fptr = fopen("log.txt", "w");
     if (fptr == NULL)
     {
         exit(-1);
     }


     if (*headPtr==NULL)
     {
         printf("The list is empty");
     }
     else{

         temp = (*headPtr);

         while (temp != NULL)
         {
            fprintf(fptr, "Fired at %c%d %s %s \n", temp->letter, temp->number, temp->state, temp->ship);

            temp = temp->next;


         }

         free(temp);

     }
     fclose(fptr);
     close(connection);
    return 0;
}




void main(int argc, char **argv)
{
    if (argc != 3 && argc != 2) { printf ("usage: battleship [ipaddress] port\n"); return; }
    else {
        srand(time(NULL));
        counter = 0;
        char state[] = "GAME START";
        char flag[] = "GAME OVER!";
        board = initialization(argc, argv);
        //struct move *head, *tail, *ourMove;
        //head = tail = NULL;

        //if (argc == 3) {
        // if there are two command line arguments, where
        // first is the ipaddress and
        // second is the port, then we initialize
        // the server side in initialization() function
        //strcpy(ipAddress,argv[1]);
        //strcpy(port,argv[2]);
        //}
        //else {
        // if there is only one command line argument,
        // then we initialize the client side in initialization
        // function
        //memset(ipAddress,0,200);
        //strcpy(port,argv[1]);
        //}
        display_state(state, board);

        if (isServer)
            serverUpDown(connection);
        else
            clientUpDown(connection);


    }

    teardown();

}