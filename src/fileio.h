#ifndef FILEIO_H_INCLUDED
#define FILEIO_H_INCLUDED

#define NAMEBUFF 20
#define MAXLINELENGTH 60
#define DELIMITER '\t'
#define CONSTnumOfUniqueCities 100

typedef struct List List;
typedef struct ListNode ListNode;

//struct ListNode
//{
//    char startCity[NAMEBUFF];
//    char destination[NAMEBUFF];
//    int energy;
//
//    ListNode* next;
//    ListNode* prev;
//};

struct ListNode
{
    int source;
    int destination;
    int energy;

    ListNode* next;
    ListNode* prev;
};

struct List
{
    int size;

    ListNode* head;
    ListNode* tail;
};

bool verbose;

int numOfUniqueCities;
char cityLookup[CONSTnumOfUniqueCities][NAMEBUFF];

/* Allocates memory for a list node */
ListNode* listNodeConstructor(char value);

/* Frees the memory for a given list node */
void listNodeDestructor(ListNode* node);

/* Allocates memory for a linked list
Returns the list to be used in the program */
List* listConstructor();

/* Frees the memory for a given linked list */
void listDestructor(List* list);

/* Imports the energy text file and sorts it into a linked list
where each node is one line in the text file (src dest energy) */
void importEnergyFile(List* list, char* energyFile);

/* Imports the citypairs text file and sorts it into a linked list
where each node is one line in the text file (start city, destination city) */
void importCitypairsFile(List* citypairsList, char* citypairsFile);

/* Reads in one line in a given text file and stores it in an array
The array is returned from the function to be used
Function contains test cases and prints to the console if the text file doesn't
exist or is at the end of the file*/
char *readLine(FILE *fp);

/* Function to add one line in a character array into a node in the energy linked list
Takes the letter array returned from the readLine function and a pointer to the linked list
Will convert all city name strings into integers and add new unique cities into the lookup array */
void listAdd(char (letter)[], List* list);

/* Function to add one line in a character array into a node in the citypairs linked list
Takes the letter array returned from the readLine function and a pointer to the linked list
Will convert all city name strings into integers using the lookup array previously created
Implementation is very similar to previous function but takes an array with no energy integer */
void listAddCitypairs(char (letter)[], List* citypairsList);

/* Takes a string which is the name of a city, and the length of the city name
Adds the city into the lookup array to give it an integer value representing the city
Returns the index to the city in the array
Uses the stringLength to fill out the rest of the array with NULL characters */
int addCityToLookup(char cityName[], int stringLength);

/* Returns the index and the integer associated with the city name given in the function arguments */
int findCityIndex(char cityName[]);

/* Returns the energy as an integer when given in a char array
Iterates over the full array and concatenates any of the numbers
or characters found that are specified */
int getEnergy(char array[]);

/* Returns the city string when given a city integer index */
char *returnCity(const int cityIndex);

/* Prints the city at a given index */
void printCity(const int cityIndex);

/* Prints out the full contents of a linked list node */
void printNode(ListNode* cityPairNode);

/* Print out the given linked list to the terminal in an easy to read format */
void listDisplay(List* list);

/* Debug function which takes the energy linked list, a city name and an integer pointer
The function searches through the energy list and returns the last index of the city
The value at the pointer cityOccurances is updated with the number of times the city is in the linked list*/
int listSearch(List* list, char city[], int *cityOccurances);

/* Returns an integer corresponding to the number of unique cities in the cityLookup array */
int numberOfUniqueCities();

/* Returns the size of the linked list given
Equal to the number of nodes within the list */
int listSize(List* list);

//Array* sort(List* list, Array* array);


#endif // FILEIO_H_INCLUDED
