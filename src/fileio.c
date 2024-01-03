#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "fileio.h"
#include "lookup.h"
#include "listGraph.h"

/* Allocates memory for a list node */
ListNode* listNodeConstructor(char value)
{
    ListNode* newNode;

    newNode = malloc(sizeof(ListNode));

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

/* Frees the memory for a given list node */
void listNodeDestructor(ListNode* node)
{
    free(node);
}

/* Allocates memory for a linked list
Returns the list to be used in the program */
List* listConstructor()
{
    List* linkedList;

    linkedList = malloc(sizeof(List));

    linkedList->size = 0;
    linkedList->head = NULL;
    linkedList->tail = NULL;

    return linkedList;
}

/* Frees the memory for a given linked list */
void listDestructor(List* list)
{
    ListNode* currentNode;
    ListNode* nextNode = list->head;

    while (nextNode != NULL)
    {
        currentNode = nextNode;
        nextNode = currentNode->next;
        listNodeDestructor(currentNode);
    }

    free(list);
}

/* Imports the energy text file and sorts it into a linked list
where each node is one line in the text file (src dest energy) */
void importEnergyFile(List* list, char* energyFile)
{
    int arrayPosition = 0;
    int i = 0;
    char *fileColumnArray;
    //char letter[MAXLINELENGTH];
    int numLines = 0;
    bool endOfFile = false;

    FILE *fp;

    for(int j = 0; j<100; j++)
    {
        cityLookup[j][0] = '\0';
    }

    fp = fopen(energyFile, "r");
    //fp = fopen("files/4_cities.txt", "r");

   while (endOfFile != true)
   {
        if (verbose == true)
        {
            printf("\n\n========"
                    "\nNew Line\n"
                    "========\n\n");
        }

        fileColumnArray = readLine(fp);

        if (fileColumnArray[0] == '\0')
        {
            if (verbose == true)
            {
                printf("End of File in importFile func\n");
            }
            memset(fileColumnArray, '\0', sizeof(fileColumnArray));
            endOfFile = true;
            break;
        }

        listAdd(fileColumnArray, list);

        i++;
   }
    for(int j = 0; j<24; j++)
    {
        printCity(j);
    }

    if (verbose == true)
    {
        printf("\nListSize: %d\n", list->size);
    }

    numLines++;

    fclose(fp);
}

/* Imports the citypairs text file and sorts it into a linked list
where each node is one line in the text file (start city, destination city) */
void importCitypairsFile(List* citypairsList, char* citypairsFile)
{
    int arrayPosition = 0;
    int i = 0;
    char *fileColumnArray;
    //char letter[MAXLINELENGTH];
    int numLines = 0;
    bool endOfFile = false;

    FILE *citypairsFP;

    citypairsFP = fopen(citypairsFile, "r");

   while (endOfFile != true)
   {
        if (verbose == true)
        {
            printf("\n\n========"
                    "\nNew Line\n"
                    "========\n\n");
        }

        fileColumnArray = readLine(citypairsFP);

        if (fileColumnArray[0] == '\0')
        {
            if (verbose == true)
            {
                printf("End of File in importFile func\n");
            }
            memset(fileColumnArray, '\0', sizeof(fileColumnArray));
            endOfFile = true;
            break;
        }


        listAddCitypairs(fileColumnArray, citypairsList);

        i++;
   }

    if (verbose == true)
    {
        printf("\nListSize: %d\n", citypairsList->size);
    }

    numLines++;

    fclose(citypairsFP);
}

/* Reads in one line in a given text file and stores it in an array
The array is returned from the function to be used
Function contains test cases and prints to the console if the text file doesn't
exist or is at the end of the file*/
char *readLine(FILE *fp)
{
    static char letter[MAXLINELENGTH];
    int arrayPosition = 0;
    int i = 0;
    bool newLineFound = false;

    letter[0] = '\0';

    if (fp == NULL)
    {
        printf("FILE EMPTY/DOES NOT EXIST\n");
        exit(-1);
    }

    while (newLineFound != true)
    {
        letter[arrayPosition] = fgetc(fp);

        if (letter[arrayPosition] == EOF)
        {
            if(feof(fp))
            {
                if (verbose == true)
                {
                    printf("End of File in readLine func\n");
                }
                letter[0] = '\0';
                newLineFound = true;
            }
            else
                printf("Error");
        }
        else
        {
            if (verbose == true)
            {
                printf("%c", letter[arrayPosition]);
            }

            if(letter[arrayPosition] == DELIMITER && verbose == true)
            {
                printf("\n");
            }
            else if(letter[arrayPosition] == '\n')
            {
                letter[arrayPosition] = '\0';
                if (verbose == true)
                {
                    printf("\n");
                }
                newLineFound = true;
            }
            arrayPosition++;
        }

    }
    return letter;
}

/* Function to add one line in a character array into a node in the energy linked list
Takes the letter array returned from the readLine function and a pointer to the linked list
Will convert all city name strings into integers and add new unique cities into the lookup array */
void listAdd(char (letter)[], List* list)
{
    int arrayPosition = 0;
    int startLength = 0;
    int destinationLength = 0;
    int energyLength = 0;
    char letterTemp;
    int i = 0;
    int nodeArrayPos = 0;
    char cityName[NAMEBUFF];
    int cityIndex = -1;

    cityName[0] = '\0';

    /* Create new node for current line */
    ListNode* newNode;
    newNode = listNodeConstructor(letterTemp);

    /* Increment List size because new node created */
    list->size++;

    /* Link the nodes together */
    if (list->size == 1)
    {
        list->head = newNode;
        list->tail = newNode;
    }
    else if (list->size > 1)
    {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }

    /* Read through array until delimiter reached
    and add start city into linked list */
    nodeArrayPos = 0;
    while (letter[arrayPosition] != DELIMITER)
    {
        cityName[nodeArrayPos] = letter[arrayPosition];
        nodeArrayPos++;
        arrayPosition++;
    }
    startLength = nodeArrayPos;


    /* Add city onto lookup array and
    returns the index to the city */
    newNode->source = addCityToLookup(cityName, startLength);

    /* Jump over tab space in array */
    arrayPosition++;

    /* Read through array from last point until next delimiter reached
    and add destination city into linked list */
    nodeArrayPos = 0;
    while (letter[arrayPosition] != DELIMITER)
    {
        cityName[nodeArrayPos] = letter[arrayPosition];
        nodeArrayPos++;
        arrayPosition++;
    }
    destinationLength = nodeArrayPos;

    /* Add city onto lookup array and
    returns the index to the city */
    newNode->destination = addCityToLookup(cityName, destinationLength);


    /* Jump over tab space in array */
    arrayPosition++;

    /* Iterate through remaining characters in line
    Add number of characters remaining to the energy length */
    while (letter[arrayPosition] != '\0')
    {
        energyLength = arrayPosition - (destinationLength+startLength+1);
        arrayPosition++;
    }

    /* Convert char number into int
    Store in linked list */
    newNode->energy = getEnergy(letter);



    if (verbose == true)
    {
        printNode(newNode);
        printf("startLength: %d\n", startLength);
        printf("destinationLength: %d\n", destinationLength);
        printf("energyLength: %d\n", energyLength);
    }

}

/* Function to add one line in a character array into a node in the citypairs linked list
Takes the letter array returned from the readLine function and a pointer to the linked list
Will convert all city name strings into integers using the lookup array previously created
Implementation is very similar to previous function but takes an array with no energy integer */
void listAddCitypairs(char (letter)[], List* citypairsList)
{
    int arrayPosition = 0;
    int startLength = 0;
    int destinationLength = 0;
    int energyLength = 0;
    char letterTemp;
    int i = 0;
    int nodeArrayPos = 0;
    char cityName[NAMEBUFF];
    int cityIndex = -1;

    cityName[0] = '\0';

    /* Create new node for current line */
    ListNode* newNode;
    newNode = listNodeConstructor(letterTemp);

    /* Increment List size because new node created */
    citypairsList->size++;

    /* Link the nodes together */
    if (citypairsList->size == 1)
    {
        citypairsList->head = newNode;
        citypairsList->tail = newNode;
    }
    else if (citypairsList->size > 1)
    {
        citypairsList->tail->next = newNode;
        newNode->prev = citypairsList->tail;
        citypairsList->tail = newNode;
    }

    /* Read through array until delimiter reached
    and add start city into linked list */
    nodeArrayPos = 0;
    while (letter[arrayPosition] != DELIMITER)
    {
        cityName[nodeArrayPos] = letter[arrayPosition];
        nodeArrayPos++;
        arrayPosition++;
    }
    startLength = nodeArrayPos;
    cityName[nodeArrayPos] = '\0';


    /* Add city onto lookup array and
    returns the index to the city */
    //newNode->source = addCityToLookup(cityName, startLength);
    newNode->source = findCityIndex(cityName);

    /* Jump over tab space in array */
    arrayPosition++;

    /* Read through array from last point until end of line reached
    and add destination city into linked list */
    nodeArrayPos = 0;
    while (letter[arrayPosition] != '\0')
    {
        cityName[nodeArrayPos] = letter[arrayPosition];
        nodeArrayPos++;
        arrayPosition++;
    }
    destinationLength = nodeArrayPos;
    cityName[nodeArrayPos] = '\0';

    /* Add city onto lookup array and
    returns the index to the city */
    //newNode->destination = addCityToLookup(cityName, destinationLength);
    newNode->destination = findCityIndex(cityName);

    /* Convert char number into int
    Store in linked list */
    newNode->energy = 0;

    if (verbose == true)
    {
        printNode(newNode);
        printf("startLength: %d\n", startLength);
        printf("destinationLength: %d\n", destinationLength);
        printf("energyLength: %d\n", energyLength);
    }

}

/* Takes a string which is the name of a city, and the length of the city name
Adds the city into the lookup array to give it an integer value representing the city
Returns the index to the city in the array
Uses the stringLength to fill out the rest of the array with NULL characters */
int addCityToLookup(char cityName[], int stringLength)
{
    int cityIndex = 0;
    bool endOfList = false;

    /* Fill rest of cityName array with NULL characters */
    for (int i = stringLength; i<=NAMEBUFF; i++)
    {
        cityName[i] = '\0';
    }

    while (endOfList == false)
    {
        if (cityLookup[cityIndex][0] == '\0')
        {
            strcpy(cityLookup[cityIndex], cityName);
            endOfList = true;
        }
        else
        {
            if(strcmp(cityLookup[cityIndex], cityName) == 0)
            {
                endOfList = true;
            }
        }
        if (cityIndex >= CONSTnumOfUniqueCities)
        {
            endOfList = true;
        }
        if (endOfList != true)
        {
            cityIndex++;
        }
    }

    return cityIndex;
}

/* Returns the index and the integer associated with the city name given in the function arguments */
int findCityIndex(char cityName[])
{
    int cityIndex = 0;
    bool endOfList = false;

    while (endOfList == false)
    {
        if (cityLookup[cityIndex][0] == '\0')
        {
            cityIndex = -1;
            endOfList = true;
        }
        else
        {
            if(strcmp(cityLookup[cityIndex], cityName) == 0)
            {
                endOfList = true;
            }
        }
        if (cityIndex >= CONSTnumOfUniqueCities)
        {
            cityIndex = -1;
            endOfList = true;
        }
        if (endOfList != true)
        {
            cityIndex++;
        }
    }

    return cityIndex;
}

/* Returns the energy as an integer when given in a char array
Iterates over the full array and concatenates any of the numbers
or characters found that are specified */
int getEnergy(char array[])
{
    int num;
    /* //p in strtol
    Pointer of position where a character that can not be interpreted appeared.
    point to '*' after execution. */
    char *p;

    num = strtol(array + strcspn(array, "-0123456789"), &p, 10);

    //printf("%d\n", num);

    return num;
}

/* Returns the city string when given a city integer index */
char *returnCity(const int cityIndex)
{
    return cityLookup[cityIndex];
}

/* Prints the city at a given index */
void printCity(const int cityIndex)
{
    if(cityLookup[cityIndex][0] == '\0')
    {
        return;
    }
    /* Print out index given and city at that index */
    printf("\nCity at Index: ");

    printf("%d - %s", cityIndex, cityLookup[cityIndex]);
}

/* Prints out the full contents of a linked list node */
void printNode(ListNode* cityPairNode)
{
    /* Dont print anything if the node received is NULL */
    if(cityPairNode == NULL)
    {
        return;
    }
    /* Print out given city pairs node */
    printf("\nSource: %s", returnCity(cityPairNode->source));
    printf("\nDestination: %s", returnCity(cityPairNode->destination));
    printf("\nEnergy: %d\n", cityPairNode->energy);
}

/* Print out the given linked list to the terminal in an easy to read format */
void listDisplay(List* list)
{
    ListNode* currentNode;
    ListNode* nextNode = list->head;

    printf("\n\n\n\n\nDISPLAYING LINKED LIST\n\n");

    /* Iterates through each node until the end is reached
    Call the printNode function to print out each node sequentially */
    while (nextNode != NULL)
    {
        currentNode = nextNode;
        nextNode = currentNode->next;

        printNode(currentNode);
    }
}

/* Debug function which takes the energy linked list, a city name and an integer pointer
The function searches through the energy list and returns the last index of the city
The value at the pointer cityOccurances is updated with the number of times the city is in the linked list*/
int listSearch(List* list, char city[], int *cityOccurances)
{
    int index = 1;
    int retIndex = -1;
    int cityIndex = -1;
    ListNode* currentNode;
    ListNode* nextNode = list->head;

    cityIndex = findCityIndex(city);

    if (cityIndex < 0)
    {
        return retIndex;
    }

    while (index <= list->size)
    {
        currentNode = nextNode;
        nextNode = currentNode->next;

        if (currentNode->source == cityIndex || currentNode->destination == cityIndex)
        {
            retIndex = index;
            printf("\n\nCity Node Index: %d\n", retIndex);
            *cityOccurances = *cityOccurances + 1;
        }

        index++;
    }


    return retIndex;
}

/* Returns an integer corresponding to the number of unique cities in the cityLookup array */
int numberOfUniqueCities()
{
    int cityIndex = 0;
    bool endOfList = false;

    while (endOfList == false)
    {
        if (cityLookup[cityIndex][0] == '\0')
        {
            endOfList = true;
        }
        if (cityIndex >= CONSTnumOfUniqueCities)
        {
            cityIndex = -1;
            endOfList = true;
        }
        if (endOfList != true)
        {
            cityIndex++;
        }
    }

    return cityIndex;
}

/* Returns the size of the linked list given
Equal to the number of nodes within the list */
int listSize(List* list)
{
    return list->size;
}


