//
//  ICSI 333. Programming at the Hardware-Software Interface
//  SEMESTER: FALL 2019
//  LAB SECTION: FRI 1:40PM
//  NAME: Gabriel Santiago
//  UA ID: 001316068
//
//
//  battleshipP2.c
//
//  Created by Gabriel Santiago on 10/15/19.
//  Copyright © 2019 Gabriel Santiago. All rights reserved.
//
//  battleshipP2 is the second part of the game. It generates a 2D Dynamic Array
//  board that will take users input and will print out the board and the state
//  of the attack!
//


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
  * Method will initialize the 2D Dynamic array which will be our playing board
  * and will set the positions of the boats in it. After it does this, it will
  * return the 2D dynamic array back to the main method
  *
  * @return board This will return the board back to the main after init
  */
int **init()
{
    // Turns off buffer for stdout (resolves error for when scanf deploys before printf)
    setbuf(stdout, NULL);


    int i;

    /*
     *  Init of the 2D Array. The board will be 10 x 10 since thats the size
     *  of a Battleship board.
     */
    int **board = malloc(10 * sizeof(int *));
    for(i = 0; i < 10; i++)
        {
            board[i] = malloc(10 * sizeof(int));
        }
     //  Carrier
    for (int x = 0; x != 5; x++)
    {
     board[1][x] = 1;
    }

    //  Battleship
    for (int y = 5; y != 9; y++)
    {
     board[y][3] = 1;
    }

    //  Cruiser
    for (int x = 7; x != 4; x--)
    {
     board[6][x] = 1;
    }

    //  Sub
    for (int x = 2; x != 5; x++)
    {
     board[x][6] = 1;
    }

    //  Destroyer
    for (int x = 4; x!=6; x++)
    {
     board[9][x] = 1;
    }

    return board;
}

/**
  * Function will take the users input and change it from a char to an int since
  * the board is in int for [x][y] coordinates when attacking. After changing
  * it will return it back to the acceptInput via pointer
  *
  */
void setXpane(int* a)
    {

        char letter;
        printf("Enter letter [A-J]\n");
        scanf(" %c%*c", &letter);


        letter = tolower(letter);

        while ((letter < 'a') || (letter > 'j'))
        {
            printf("\nInvalid Letter. Please enter a Letter between A and J:\n");

            //  Fixes auto-enter or  annoying "\n" from the in
            scanf(" %c%*c", &letter);
        }

        switch(letter)
        {
            case 'a': *a = 0;
            break;

            case 'b': *a = 1;
            break;

            case 'c': *a = 2;
            break;

            case 'd': *a = 3;
            break;

            case 'e': *a = 4;
            break;

            case 'f': *a = 5;
            break;

            case 'g': *a = 6;
            break;

            case 'h': *a = 7;
            break;

            case 'i': *a = 8;
            break;

            case 'j': *a = 9;
            break;
        }
    }

/**
  * Will take users input letter and number and will return them back to the main
  * Inside of this method, it calls another method called setXpane that will
  * return the int value of the entered char, return it to this method and then
  * the method will take that and return it to the main via pointer
  *
  * NOTE: I know its kind of a mess but its been tested and it works perectly
  */
void acceptInput(int* letter, int* number)
{
  //  Gets the int value of the char
  setXpane(letter);
  printf("\n");

  //  Prompts user to enter number within range
  printf("Enter number [1-9]\n");
  scanf("%d", number);

  while ((*number < 0) || (*number > 9))
  {
    printf("Invalid input, please enter numbers from [1 - 9]\n");
    scanf("%d", number);
  }
}

/**
  * Will take the board coordinates and determine if there is a 1 stored inside
  * of it. If there is then it will set the world state to Hit! If not then Miss!
  *
  * @param *world Pointer that will return the world state to main
  * @param letter is the letter entered by the user
  * @param number is the number entered by the user
  * @param **board is the double pointer that recieves the board for the condit
  *
  * @return world Returns the state of the world
  */
char * updateState(char *world, int letter, int number, int **board)
{


  if (board[letter][number] == 1)
        {
            world = "Hit!";
            return world;
        }
        else
        {
            world = "Miss!";
            return world;
        }
}
/**
  * This method will display to the user the state of the world as well as the
  * graphical reprentation of the game board with the positions of the boats.
  * The boat pos is represented by 1 and 0 for empty spaces.
  *
  * @param world This pointer will recieve the world status that will be later
  * printed for the user
  * @param **board Double pointer of the board that will be used to print out the
  * graphical representation of our board
  */
void displayState(char* world, int **board)
{
  //  Will print out the board
  printf("2D display\n");
  printf("   ");

  //  Actually prints the board
  for(int n = 1; n != 11; n++)  //  This will print the first row of numbers
      {
        printf("%d ", n);
      }

    printf("\n");
    printf("   ===================\n");

    int charAsInt = 97; //  Starts at the char value 97 which is 'a'.
    //  Will be used to print out the letters of the board

    //  Horizontal
    for (int x = 0; x != 10; x++)
      {
        //  Will print out the char representations of the int value
        //  This will represent the letters on the board
        printf("%c", charAsInt);

        printf("| ");

        //  Vertical
        for (int y = 0; y != 10; y++)
         {
            //  Prints out board
            printf("%d ", board[x][y]);
            //  When y reaches 9 then it will go to the next line and print
            if (y == 9)
              printf("\n");
         }
        // Increments the char value by 1
        charAsInt++;
        }

    printf("\n");
    printf("%s\n", world); // Displays the world status
}

/**
  * Will terminate the board aka free up the allocated
  *
  * @param **board double pointer that will be the board that will be freed
  */
void term(int **board)
{
    for(int i = 0; i < 10; i++)
		free(board[i]);
	free(board);
	printf("\nBoard was cleared!");
}

//  Main Method
int main()
{
  //  Declaration of varaibles
  int number, letter;
  char* world;
  int flag;

  do
  {
    //  Init the board
    int **board = init();
    acceptInput(&letter, &number);

    world = updateState(world, letter, number, board);
    printf("\n");
    displayState(world, board);

    //  Terminates the board
    term(board);

    printf("\n");
    //  Ask user if they want to play again
    printf("Enter '1' to start over and '0' to terminate\n");
    scanf("%d", &flag);


  }while(flag == 1);


    return 0;
}
