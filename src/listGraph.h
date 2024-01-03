#ifndef LISTGRAPH_H_INCLUDED
#define LISTGRAPH_H_INCLUDED

#include "fileio.h"


#define INFINITY 99999

typedef struct adjListNode adjListNode;
typedef struct adjList adjList;
typedef struct Graph Graph;

// Structure to represent an adjacency list node
struct adjListNode
{
    struct adjListNode *next;
    int dest;
    int energy;
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
    int edgeNum;
    adjList *connectedCities;
};

adjListNode* adjListNodeConstructor(int dest, int energy);

// Creates a graph with "vertexNum" number of vertices
Graph* graphConstructor(int vertexNum, int edgeNum);

void adjListNodeDestructor(adjListNode* node);

void graphDestructor(Graph* graph);

void addEdgeAdjList(Graph* graph, int src, int dest, int energy, char* isDirected);

void addEdgeAdjListUndirected(Graph* graph, int src, int dest, int energy);

void addEdgeAdjListDirected(Graph* graph, int src, int dest, int energy);

void printGraph(Graph* graph);

int bellmanford(Graph* graph, int startSrc, int pathDestination);

void displayResult(int arr[], int size);


#endif // LISTGRAPH_H_INCLUDED
