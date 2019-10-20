#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int **init()
{
    setbuf(stdout, NULL);
    // Creation of string variable type pointer so it isnt constant
    int i;
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

void setXpane(int* a)
    {

        char letter;
        printf("Enter letter [A-J]\n");
        scanf(" %c%*c", &letter);


        letter = tolower(letter);

        while ((letter < 'a') || (letter > 'j'))
        {
            printf("\nInvalid Letter. Please enter a Letter between A and J:\n");
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

void acceptInput(int* letter, int* number)
{
  setXpane(letter);
  printf("\n");
  printf("Enter number [1-9]\n");
  scanf("%d", number);

  while ((*number < 0) || (*number > 9))
  {
    printf("Invalid input, please enter numbers from [1 - 9]\n");
    scanf("%d", number);
  }
}

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

void displayState(char* world, int **board)
{
  printf("2D display\n");
  printf("   ");
  for(int n = 1; n != 11; n++)
      {
        printf("%d ", n);
      }
    printf("\n");
    printf("   ===================\n");

    int charAsInt = 97;

    for (int x = 0; x != 10; x++)
      {

        printf("%c", charAsInt);

        printf("| ");

        for (int y = 0; y != 10; y++)
         {
            printf("%d ", board[x][y]);
            if (y == 9)
              printf("\n");
         }

        charAsInt++;
        }
    printf("\n");
    printf("%s\n", world);
}

void term(int **board)
{
    for(int i = 0; i < 10; i++)
		free(board[i]);
	free(board);
	printf("\nBoard was cleared!");
}

int main()
{
  int number, letter;
  char* world;
  int flag;

  do
  {

    int **board = init();
    acceptInput(&letter, &number);

    world = updateState(world, letter, number, board);
    printf("\n");
    displayState(world, board);
    term(board);

    printf("\n");

    printf("Enter '1' to start over and '0' to terminate\n");
    scanf("%d", &flag);


  }while(flag == 1);


    return 0;
}
