#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "fileio.h"
#include "lookup.h"


#define INFINITY 99999

// Initialize the matrix to infinity
void initialiseAdjMatrix()
{
    for (int i = 0; i < numOfUniqueCities; i++)
    {
        for (int j = 0; j < numOfUniqueCities; j++)
        {
            if (i == j)
            {
                adjMatrix[i][j] = 0;
            }
            else
            {
                adjMatrix[i][j] = INFINITY;
            }
        }
    }
}

/* Initialises the matrix with the known values imported from the text file */
void initialiseKnownEdges(List* list, char* isDirected)
{
    ListNode* currentNode;
    ListNode* nextNode = list->head;

    while (nextNode != NULL)
    {
        currentNode = nextNode;
        nextNode = currentNode->next;

        addEdge(currentNode->source, currentNode->destination, currentNode->energy, isDirected);
    }
}

/* Add edge between source and destination
Checks the direction specified for the graph
Adds the edges in the direction specified by reversing the src and dest parameters given */
void addEdge(int src, int dest, int energy, char* isDirected)
{
    if (strcmp(isDirected, "undir") == 0)
    {
        adjMatrix[src][dest] = energy;
        adjMatrix[dest][src] = energy;
    }
    else if (strcmp(isDirected, "dir") == 0)
    {
        adjMatrix[src][dest] = energy;
    }
    else if (strcmp(isDirected, "revdir") == 0)
    {
        adjMatrix[dest][src] = energy;
    }
    else if (strcmp(isDirected, "-vedir") == 0)
    {
        if (energy >= 0)
        {
            adjMatrix[src][dest] = energy;
            adjMatrix[dest][src] = energy;
        }
        else
        {
            adjMatrix[src][dest] = energy;
        }
    }
    else if (strcmp(isDirected, "-verevdir") == 0)
    {
        if (energy >= 0)
        {
            adjMatrix[src][dest] = energy;
            adjMatrix[dest][src] = energy;
        }
        else
        {
            adjMatrix[dest][src] = energy;
        }
    }
    else
    {
        printf("No direction defined in Floyd Warshall graph\n");
    }
}

/* Initialises the path matrix which is filled with j's value if there is an edge
Filled with -1 if there is no edge to start with */
void initialisePathMatrix()
{
    // Initialize the path matrix
    for (int i = 0; i < numOfUniqueCities; i++)
    {
        for (int j = 0; j < numOfUniqueCities; j++)
        {
            //Initialize adjMatrixPath[i][j] to j if there is an edge between i and j, otherwise set it to -1
            if (adjMatrix[i][j] != 0)
            {
                adjMatrixPath[i][j] = j;
            }
            else
            {
                adjMatrixPath[i][j] = -1;
            }
        }
    }
}

/* Returns the edge weight i.e. the energy between a src and destination given */
float readEdgeWeight(int src, int dest)
{
    float edgeWeight = INFINITY;
    edgeWeight = adjMatrixFinal[src][dest];

    return edgeWeight;
}

/* Prints the matrix in a formatted view to make checking values quick and easy */
void printAdjMatrix()
{
    //int width = snprintf(NULL, 0, "%g", adjMatrix[0][1]);

    int width = 6;

    printf("\n");
    for (int i = 0; i < numOfUniqueCities; i++)
    {
        printf("%3d - %11s: ", i, returnCity(i));
        printf("[");

        for (int j = 0; j < numOfUniqueCities; j++)
        {
            if (j != 0) printf(", ");
            printf("%*g", width, adjMatrix[i][j]);
        }
        printf("]\n");
    }
    printf("\n");

}

/* Function override equivalent for printAdjMatrix
Used to print the final matrix when the -vedir direction is specified */
void printAdjMatrixFinal()
{
    //int width = snprintf(NULL, 0, "%g", adjMatrix[0][1]);

    int width = 6;

    printf("\n");
    for (int i = 0; i < numOfUniqueCities; i++)
    {
        printf("%3d - %11s: ", i, returnCity(i));
        printf("[");

        for (int j = 0; j < numOfUniqueCities; j++)
        {
            if (j != 0) printf(", ");
            printf("%*g", width, adjMatrixFinal[i][j]);
        }
        printf("]\n");
    }
    printf("\n");

}

/* Prints out the path matrix to show which edges have been relaxed and updated
If any edges remain with -1 at the end, then there is no path between those two cities at all */
void printPathMatrix()
{
    //int width = snprintf(NULL, 0, "%g", adjMatrix[0][1]);

    int width = 6;

    printf("\n");
    for (int i = 0; i < numOfUniqueCities; i++)
    {
        printf("%3d - %11s: ", i, returnCity(i));
        printf("[");

        for (int j = 0; j < numOfUniqueCities; j++)
        {
            if (j != 0) printf(", ");
            printf("%*g", width, adjMatrixPath[i][j]);
        }
        printf("]\n");
    }
    printf("\n");

}

/* Main function to run the algorithm
If the path through an intermediate city from source to destination
is less than the path directly from the source to destination
then the distance is updated in the graph matrix */
void findShortestPath()
{
    float temp = 0;
    for (int k = 0; k < numOfUniqueCities; k++)
    {
        for (int i = 0; i < numOfUniqueCities; i++)
        {
            for (int j = 0; j < numOfUniqueCities; j++)
            {
                if (adjMatrix[i][k] < INFINITY && adjMatrix[k][j] < INFINITY)
                {
                    if (i != j)
                    {
                        if (!has_repeated_city(i, k, j))
                        {
                            temp = (adjMatrix[i][k] + adjMatrix[k][j]);
                            if (adjMatrix[i][j] > temp)
                            {
                                adjMatrix[i][j] = temp;
                            }
                            /*Update adjMatrixPath[i][j] to adjMatrixPath[i][k]
                            if adjMatrix[i][k] + adjMatrix[k][j] is less than adjMatrix[i][j]
                            and the path from i to k does not contain j.
                            This will ensure that the path from i to j does not contain any repeated city
                            */
                            adjMatrixPath[i][j] = adjMatrixPath[i][k];

                        }

                    }
                }

            }
        }
        if (verbose == true)
        {
            printf("\nk = %d\n", k);
            printf("Adj Matrix\n");
            printAdjMatrix();
            printf("Path Matrix\n");
            printPathMatrix();
        }

    }
}

/* Recursive function which iterates over the path matrix
If the path being tested hasnt passed through the intermediate city i.e. it is -1
then the function returns 0
Else it returns 1 to make sure that path isnt relaxed and updated */
int has_repeated_city(int i, int k, int j)
{
    if (adjMatrixPath[i][k] == -1 || adjMatrixPath[k][j] == -1)
        return 0;
    if (adjMatrixPath[i][k] == j || adjMatrixPath[k][j] == i)
        return 1;
    return has_repeated_city(i, adjMatrixPath[k][j], j) || has_repeated_city(i, k, adjMatrixPath[i][k]);
}
