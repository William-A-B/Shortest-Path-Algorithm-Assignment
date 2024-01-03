#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "fileio.h"

#define Verticies 100

#define INFINITY 99999

//float (*adjMatrix)[];

float adjMatrix[Verticies][Verticies];
float adjMatrixFinal[Verticies][Verticies];
float adjMatrixPath[Verticies][Verticies];

/* Initialize the matrix to infinity */
void initialiseAdjMatrix();

/* Initialises the matrix with the known values imported from the text file */
void initialiseKnownEdges(List* list, char* isDirected);

/* Add edge between source and destination
Checks the direction specified for the graph
Adds the edges in the direction specified by reversing the src and dest parameters given */
void addEdge(int src, int dest, int energy, char* isDirected);

/* Initialises the path matrix which is filled with j's value if there is an edge
Filled with -1 if there is no edge to start with */
void initialisePathMatrix();

/* Returns the edge weight i.e. the energy between a src and destination given */
float readEdgeWeight(int src, int dest);

/* Prints the matrix in a formatted view to make checking values quick and easy */
void printAdjMatrix();

/* Function override equivalent for printAdjMatrix
Used to print the final matrix when the -vedir direction is specified */
void printAdjMatrixFinal();

/* Prints out the path matrix to show which edges have been relaxed and updated
If any edges remain with -1 at the end, then there is no path between those two cities at all */
void printPathMatrix();

/* Main function to run the algorithm
If the path through an intermediate city from source to destination
is less than the path directly from the source to destination
then the distance is updated in the graph matrix */
void findShortestPath();

/* Recursive function which iterates over the path matrix
If the path being tested hasnt passed through the intermediate city i.e. it is -1
then the function returns 0
Else it returns 1 to make sure that path isnt relaxed and updated */
int has_repeated_city(int i, int k, int j);

#endif // GRAPH_H_INCLUDED
