//
//  ICSI 333. Programming at the Hardware-Software Interface
//  SEMESTER: FALL 2019
//  LAB SECTION: FRI 1:40PM
//  NAME: Gabriel Santiago
//  UA ID: 001316068
//
//
//  BattleshipProject1.c
//
//  Created by Gabriel Santiago on 9/25/19.
//  Copyright © 2019 Gabriel Santiago. All rights reserved.



#include <stdio.h>
#include <ctype.h>




//Part 1
//Function type char pointer will set the string to empty and return it to the main
char * init()
{
    // Creation of string variable type pointer so it isnt constant
    char *str;
    str = "";
    
    //Return the string to the main
    return str;
    
}



//Part2
//Function will recieve the letter and number type pointer and set it as the input
void acceptInput(char* letter, int* number)
{
    //User input
    printf("Enter a letter A-J: \n");
    scanf("%c", letter);
    
    //Converts characters to upper case for better handling in the software and less user errors
    *letter = toupper(*letter);
    
    
    //While the letter is out of the scope A - J, if it is then it will ask user to input correct character
    while ((*letter < 'A' || *letter > 'J'))
    {
        printf("Invalid Letter. Please enter a Letter between A and J: \n");
        scanf("%c", letter);
        *letter = toupper(*letter);
    }
    
    //User input
    printf("Enter a number 0-9: \n");
    scanf("%d", number);
    
    //While the letter is out of the scope 0 - 9, if it is then it will ask user to input correct character
    while ((*number < 0) || (*number > 9))
    {
        printf("Invalid input, please enter numbers from [1 - 9]\n");
        scanf("%d", number);
    }
   
    
}

//Part 3
/*
/ Will update state of world, if user input is a vowel and a even number
/ then it will set the world as a "Hit". If it does not meet this criteria then the world
/ will be set as a "Miss". It will also return this
*/
char * updateStateOfWorld(char *world, int number, char letter)
{
    
    //Condition that will verify if number is medium and other letters are vowels
    if (number%2 == 0 && ((letter == 'A') || (letter == 'E') || (letter == 'I') || (letter == 'O') || (letter == 'U')))
    {
        world = "Hit!";
        return world;
    }
    
    else
    {
        world = "Miss";
        return world;
    }
    
}

//Part 4
//Will display the state of the world
void displayState(char* world)
{
    printf("%s\n", world);
}

//Part 5
//Empty for now
void termination()
{
    
}

//Main funciton
int main()
{
    int number;
    char letter;
    char *world = init();
    int flag = 1;
    char choice;
    
    //Part 6
    do
    {
        //Caling of acceptInput
        acceptInput(&letter, &number);
        
        //Displays user letter and number entered
        printf("%c", letter);
        
        printf("%d\n", number);
        
        //Sets the world to hit or miss
        world = updateStateOfWorld(world, number, letter);
        
        //Displays the hit or miss
        displayState(world);
        
        //Conditions to end the program, if the user enters p then it will set the flag to either 1 or 0
        //if the flag is 0 then it will terminate the program
        printf("Enter 'p' to finish\n");
        scanf("%s", &choice);
        choice = toupper(choice);
        if (choice == 'P')
        {
            flag = 0;
        }
        else
        {
            flag = 1;
        }
        
    }while(flag == 1);
    
    //Call of termination
    termination();
    
    return 0;
}
