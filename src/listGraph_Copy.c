#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "fileio.h"
#include "lookup.h"
#include "listGraph.h"

#define MAXNODE 4

adjList *mygList[MAXNODE] = {0};

adjListNode* adjListNodeConstructor(int dest, int energy)
{
    adjListNode* newNode;

    newNode = malloc(sizeof(adjListNode));
    newNode->dest = dest;
    newNode->energy = energy;
    newNode->next = NULL;

    return newNode;
}

// Creates a Graph with "vertexNum" number of vertices
Graph* graphConstructor(int vertexNum, int edgeNum)
{
    Graph* graph;

    graph = malloc(sizeof(Graph));
    graph->vertexNum = vertexNum;
    graph->edgeNum = edgeNum;

    // Create an array of adjacency lists
    // Size of array will equal vertexNum
    graph->connectedCities = malloc(vertexNum * sizeof(adjList));

    // Initialise each adjacency list as empty by setting each
    // head to be NULL
    for(int i = 0; i < vertexNum; ++i)
    {
        graph->connectedCities[i].head = NULL;
    }

    return graph;
}
/* Direction of graph selection for Bellman Ford algorithm
Uses isDirected to determine which direction the edges will be added into the graph */
void addEdgeAdjList(Graph* graph, int src, int dest, int energy, char* isDirected)
{

    if (strcmp(isDirected, "undir") == 0)
    {
        /* Adds edges to be undirected */
        addEdgeAdjListUndirected(graph, src, dest, energy);
    }
    else if (strcmp(isDirected, "dir") == 0)
    {
        /* Adds edges to be directed from src to dest */
        addEdgeAdjListForwardsDirected(graph, src, dest, energy);
    }
    else if (strcmp(isDirected, "revdir") == 0)
    {
        /* Adds edges to be directed from dest to src */
        addEdgeAdjListReverseDirected(graph, src, dest, energy);
    }
    else
    {
        /* No direction or incorrect direction defined in command line arguments */
        printf("No direction defined in Bellman Ford graph\n");
    }
}

void addEdgeAdjListUndirected(Graph* graph, int src, int dest, int energy)
{
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning
    adjListNode* check = NULL;
    adjListNode* newNode = adjListNodeConstructor(dest, energy);

    if (graph->connectedCities[src].head == NULL)
    {
        newNode->next = graph->connectedCities[src].head;
        graph->connectedCities[src].head = newNode;
    }
    else
    {
        check = graph->connectedCities[src].head;
        while (check->next != NULL)
        {
            check = check->next;
        }
        check->next = newNode;
    }

    // Since graph is undirected, add an edge from
    // dest to src also
    newNode = adjListNodeConstructor(src, energy);
    if (graph->connectedCities[dest].head == NULL)
    {
        newNode->next = graph->connectedCities[dest].head;
        graph->connectedCities[dest].head = newNode;
    }
    else
    {
        check = graph->connectedCities[dest].head;
        while (check->next != NULL)
        {
            check = check->next;
        }
        check->next = newNode;
    }
}

void addEdgeAdjListForwardsDirected(Graph* graph, int src, int dest, int energy)
{
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the beginning
    adjListNode* check = NULL;
    adjListNode* newNode = adjListNodeConstructor(dest, energy);

    if (graph->connectedCities[src].head == NULL)
    {
        newNode->next = graph->connectedCities[src].head;
        graph->connectedCities[src].head = newNode;
    }
    else
    {
        check = graph->connectedCities[src].head;
        while (check->next != NULL)
        {
            check = check->next;
        }
        check->next = newNode;
    }
}

void addEdgeAdjListReverseDirected(Graph* graph, int src, int dest, int energy)
{
    // Since graph is undirected, add an edge from
    // dest to src also
    adjListNode* check = NULL;
    adjListNode* newNode = adjListNodeConstructor(src, energy);

    if (graph->connectedCities[dest].head == NULL)
    {
        newNode->next = graph->connectedCities[dest].head;
        graph->connectedCities[dest].head = newNode;
    }
    else
    {
        check = graph->connectedCities[dest].head;
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
        adjListNode* pCrawl = graph->connectedCities[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf(" --> %d - %d", pCrawl->dest, pCrawl->energy);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}


int bellmanford(Graph* graph, int startSrc, int pathDestination)
{
    //variables
    int i, j = 0;
    int src = 0;
    int dest = 0;
    int energy = 0;
    int negativeCycleCount = 0;


    adjListNode* currentNode;
    adjListNode* nextNode = graph->connectedCities[0].head;;

    //distance array
    //size equal to the number of vertices of the graph g
    int distance[graph->vertexNum];

    //predecessor array
    //size equal to the number of vertices of the graph g
    int predecessor[graph->vertexNum];

    //step 1: fill the distance array and predecessor array
    for (i = 0; i < graph->vertexNum; i++)
    {
        distance[i] = INFINITY;
        predecessor[i] = 0;
    }

    //mark the source vertex
    distance[startSrc] = 0;

    if (verbose == true)
    {
        printf("Distance array: ");
        displayResult(distance, graph->vertexNum);
        printf("Predecessor array: ");
        displayResult(predecessor, graph->vertexNum);
        printf("\n");
    }



    //step 2: relax edges |V| - 1 times
    for (i = 1; i <= graph->vertexNum - 1; i++)
    {
        for (j = 0; j<graph->vertexNum; j++)
        {
            nextNode = graph->connectedCities[j].head;
            currentNode = nextNode;

            while (currentNode != NULL)
            {
                //get the edge data
                //src = graph->connectedCities[j];
                src = j;
                dest = currentNode->dest;
                energy = currentNode->energy;

                if (distance[src] != INFINITY && distance[dest] > distance[src] + energy)
                {
                    distance[dest] = distance[src] + energy;
                    predecessor[dest] = src;
                }

                currentNode = currentNode->next;
            }
        }
        if (verbose == true)
        {
            printf("\n%d\n", i);
            printf("Distance array: ");
            displayResult(distance, graph->vertexNum);
            printf("Predecessor array: ");
            displayResult(predecessor, graph->vertexNum);
            printf("\n");
        }

    }

    //step 3: detect negative cycle
    //if value changes then we have a negative cycle in the graph
    //and we cannot find the shortest distances
    for (j = 0; j<graph->vertexNum; j++)
    {
        nextNode = graph->connectedCities[j].head;
        currentNode = nextNode;

        while (currentNode->next != NULL)
        {
            //get the edge data
            //src = graph->connectedCities[j];
            src = j;
            dest = currentNode->dest;
            energy = currentNode->energy;

            if (distance[src] != INFINITY && distance[dest] > distance[src] + energy)
            {
                negativeCycleCount++;

            }

            currentNode = currentNode->next;
        }
    }

    if (negativeCycleCount > 1)
    {
        printf("Number of negative cycles found in graph: %d\n\n", negativeCycleCount);
    }


    //No negative weight cycle found!
    //print the distance and predecessor array
    printf("Distance array: ");
    displayResult(distance, graph->vertexNum);
    printf("Predecessor array: ");
    displayResult(predecessor, graph->vertexNum);
    printf("\n");


    return distance[pathDestination];
}



void displayResult(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}












//
//
//adjList* adjListConstructor()
//{
//    adjList* adjList;
//
//    adjList = malloc()
//}
//
//
//
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
//
//void addNode(adjList* adjList)
//{
//    int s = 0;
//    int d= 0;
//    adjListNode *destination, *temp, *source;
//
//    if(adjList[s]->head == NULL)
//    {
//        source = gNodeConstructor();
//        source->vertexNum = s;
//        source->next = NULL;
//        adjList[s]->head = source;
//
//    }
//}
