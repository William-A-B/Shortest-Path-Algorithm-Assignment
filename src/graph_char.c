#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "fileio.h"


#define MAXNODE 4


typedef struct adjListNode adjListNode;
typedef struct adjList adjList;
typedef struct Graph Graph;

// Structure to represent an adjacency list node
struct adjListNode
{
    struct adjListNode *next;
    char dest;
};

// Structure to represent an adjacency list to store the nodes
struct adjList
{
    adjListNode *head;
};

// Structure to represent a graph which is an array of adjacency lists
// Array size will be equal to the number of vertices in the graph
struct Graph
{
    int vertexNum;
    adjList *array[50][20];
//    strcpy(a[0], "blah");
//    strcpy(a[1], "hmm");
};

//adjList *mygList[MAXNODE] = {0};

adjListNode* adjListNodeConstructor(char dest)
{
    adjListNode* newNode;

    newNode = malloc(sizeof(adjListNode));
    newNode->dest = dest;
    newNode->next = NULL;

    return newNode;
}

// Creates a Graph with "vertexNum" number of vertices
Graph* graphConstructor(int vertexNum)
{
    Graph* graph;

    graph = malloc(sizeof(Graph));
    graph->vertexNum = vertexNum;

    // Create an array of adjacency lists
    // Size of array will equal vertexNum
    graph->array[vertexNum][20] = malloc(vertexNum * sizeof(adjList));

    // Initialise each adjacency list as empty by setting each
    // head to be NULL
    for(int i = 0; i < vertexNum; ++i)
    {
        graph->array[i][].head = NULL;
    }

    return graph;
}

void addEdge(Graph* graph, char src, char dest)
{
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning
    adjListNode* check = NULL;
    adjListNode* newNode = adjListNodeConstructor(dest);

    if (graph->array[src].head == NULL)
    {
        newNode->next = graph->array[src].head;
        graph->array[src].head = newNode;
    }
    else
    {
        check = graph->array[src].head;
        while (check->next != NULL)
        {
            check = check->next;
        }
        // graph->array[src].head = newNode;
        check->next = newNode;
    }

    // Since graph is undirected, add an edge from
    // dest to src also
    newNode = adjListNodeConstructor(src);
    if (graph->array[dest].head == NULL)
    {
        newNode->next = graph->array[dest].head;
        graph->array[dest].head = newNode;
    }
    else
    {
        check = graph->array[dest].head;
        while (check->next != NULL)
        {
            check = check->next;
        }
        check->next = newNode;
    }
}


void printGraph(Graph* graph)
{
    for (int v = 0; v < graph->vertexNum; ++v) {
        adjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}





//adjList* adjListConstructor()
//{
//    adjList* adjList;
//
//    adjList = malloc()
//}



//adjList* adjListConstructor()
//{
//    adjList* graphLinkedList;
//
//    graphLinkedList = malloc(sizeof(adjList));
//
//    graphLinkedList->head = NULL;
//
//    return graphLinkedList;
//}
//
//adjListNode* gNodeConstructor()
//{
//    adjListNode* newgNode;
//
//    newgNode = malloc(sizeof(newgNode));
//
////    newNode->startCity[0] = NULL;
////    newNode->destination[0] = NULL;
////    newNode->energy = NULL;
//    newgNode->next = NULL;
//
//    return newgNode;
//}

void addNode(adjList* adjList)
{
    int s = 0;
    int d= 0;
    adjListNode *destination, *temp, *source;

//    if(adjList[s]->head == NULL)
//    {
//        source = gNodeConstructor();
//        source->vertexNum = s;
//        source->next = NULL;
//        adjList[s]->head = source;
//
//    }
}
