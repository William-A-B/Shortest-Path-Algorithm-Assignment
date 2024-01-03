#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "fileio.h"
#include "lookup.h"
#include "listGraph.h"
#include <time.h>


#define MAXNODE 4

/* User function to handle the importing of the data from the text files
Adds both the energy text file into the energy linked list, as well as the
citypairs text file into the citypairs linked list.
Uses the args energyFileName and citypairsFileName to know which files to import from */
void initialiseData(List* energyList, List* citypairsList, char* energyFileName, char* citypairsFileName);

/* Handles adding the data to the graph, and calling of user functions to run the Floyd algorithm */
void runFloydWarshall(List* energyList, List* citypairsList, char* isDirected, char* outputFileName);

/* Function used for the adjacency matrix graph, initialises the energy list into the graph
Adds the data correctly corresponding to the direction state of the graph given in the command line arguments */
void addToGraph(List* energyList, char* isDirected);

/* Handles the function calls to run the Floyd Warshall algorithm to find
the shortest path between the citypairs given in citypairsList
Outputs the relevant data to the filename given in outputFileName */
void runFloydAlgorithm(List* energyList, List* citypairsList, char* outputFileName);

/* Called by Floyd algorithm as an intermediate function to get the final path energy
Then passes the path energy to printCityPairDistance which writes to the text file */
void printCitypair(ListNode* citypairNode, char* outputFileName);

/* Prints the shortest path between two given cities
Cities are passed via a linked list node
Outputs to the file named outputFileName */
void printCitypairDistance(ListNode* citypairNode, float pathEnergy, char* outputFileName);

/* Handles the function calls to run the Bellman Ford algorithm to find the
shortest path between the citypairs given in a citypairsList */
void runBellmanFord(List* energyList, List* citypairsList, char* isDirected, char* outputFileName);

/* Adds the energy list into the adjacency list graph so the bellman ford algorithm can work */
void addToAdjListGraph(Graph* myGraph, List* energyList, List* citypairsList, char *isDirected);




int main(int arg,char* argv[])
{

    /* Clock process which starts the clock timer
    Used to calculate the execution time of the program */
    clock_t start, end;
    double execution_time;
    start = clock();

    /* Char pointer arrays to store the command line arguments */
    char* energyFileName = "energy-v23-1.txt";
    char* citypairsFileName = "citypairs.txt";
    char* outputFileName = "shortestpath_results.txt";
    char* isDirected = "undir";
    char* algorithm = "bellmanford";
    char* verbose_c = "false";
    verbose = false;

    puts("Algorithms Assignment...");

    /* Conditional to store the command line arguments
    If the correct number of arguments are given,
    each index in the array is stored in the respective variables */
    if(arg>7 || arg<1)
    {
        puts("Not enough input arguments given\n");
        return EXIT_FAILURE;
    }
    else
    {
        switch(arg)
        {
            case 7: verbose_c = argv[6];
            case 6: algorithm = argv[5];
            case 5: isDirected = argv[4];
            case 4: outputFileName = argv[3];
            case 3: citypairsFileName = argv[2];
            case 2: energyFileName = argv[1];
            break;
        }
    }

    /* Check if the user has requested a verbose output */
    verbose = !strcmp(verbose_c, "t");

    if (verbose == true)
    {
        puts(argv[0]);
        puts(energyFileName);
        puts(citypairsFileName);
        puts(outputFileName);
        puts(isDirected);
        puts(algorithm);
        puts(verbose_c);
    }

    /* Output file for the results
    Clears any existing data and prepares the file for new results */
    FILE *outputFP;
    outputFP = fopen(outputFileName, "w");
    fprintf(outputFP, "Shortest path from source to destination city\nUsing %s (%s)\n\n", algorithm, isDirected);
    fclose(outputFP);


    /* Define energyList to store the source to destination city pairs */
    List* energyList;
    /* Define cityPairsList to store the city pairs
    These pairs are what the program needs to find the shortest path between */
    List* citypairsList;


    /* Allocate memory for both lists defined above */
    energyList = listConstructor();
    citypairsList = listConstructor();

    /* User function to handle the importing of the data from the text files */
    initialiseData(energyList, citypairsList, energyFileName, citypairsFileName);

    /* Conditionals for the "algorithm" command line argument
    Calls the correct algorithm functions depending on which one is requested */

    if (strcmp(algorithm, "bellmanford") == 0)
        runBellmanFord(energyList, citypairsList, isDirected, outputFileName);
    else if (strcmp(algorithm, "floydwarshall") == 0)
    {
        runFloydWarshall(energyList, citypairsList, isDirected, outputFileName);
    }
    else
        printf("No algorithm selected\n");


    /* Free the memory for the linked lists */
    listDestructor(energyList);
    listDestructor(citypairsList);


    /* Calculate the execution time and print it to the console in seconds */
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("%lf", execution_time);

    return EXIT_SUCCESS;
}

void initialiseData(List* energyList, List* citypairsList, char* energyFileName, char* citypairsFileName)
{
    //Handles import of energy file

    /* Uncomment variables to use the debug section below */
    //int cityIndex = -1;
    //int cityOccurances = 0;

    importEnergyFile(energyList, energyFileName);

    numOfUniqueCities = numberOfUniqueCities();

    printf("\n\nNum of Unique Cities Imported = %d\n\n", numOfUniqueCities);

    if (verbose == true)
    {
        listDisplay(energyList);
    }


    /* Debug used to check how many times a city name appeared in the list
    returns the last index of the city, and listSearch prints the index each
    time it finds the city given */
    //cityIndex = listSearch(energyList, "York", &cityOccurances);
    //printf("\n\nCity Occurances: %d\n", cityOccurances);

    //Handles import of citypairs file
    importCitypairsFile(citypairsList, citypairsFileName);
}

void runFloydWarshall(List* energyList, List* citypairsList, char* isDirected, char* outputFileName)
{
    addToGraph(energyList, isDirected);
    runFloydAlgorithm(energyList, citypairsList, outputFileName);
    for (int i = 0; i < numOfUniqueCities; i++)
    {
        for (int j = 0; j < numOfUniqueCities; j++)
        {
            adjMatrixFinal[i][j] = adjMatrix[i][j];
        }
    }
    /* If negative directed requested, then also run the algorithm reverse
    negative directed */
    if (strcmp(isDirected, "-vedir") == 0)
    {
        addToGraph(energyList, "-verevdir");
        runFloydAlgorithm(energyList, citypairsList, outputFileName);
        for (int i = 0; i < numOfUniqueCities; i++)
        {
            for (int j = 0; j < numOfUniqueCities; j++)
            {
                if (adjMatrix[i][j] < adjMatrixFinal[i][j])
                {
                    adjMatrixFinal[i][j] = adjMatrix[i][j];
                }
            }
        }
        /* Final matrix used so the algorithm can run twice and add the
        shortest route per city pair into the graph correctly when its -vedir */
        printf("Finalised Shortest Path Graph\n");
        printAdjMatrixFinal();
    }

    /* Writes the results to the text file output
    Text file name is given by the arg outputFileName */
    ListNode* currentNode;
    ListNode* nextNode = citypairsList->head;

    while (nextNode != NULL)
    {
        currentNode = nextNode;
        nextNode = currentNode->next;

        printCitypair(currentNode, outputFileName);
    }
}

void addToGraph(List* energyList, char* isDirected)
{
    initialiseAdjMatrix();
    initialiseKnownEdges(energyList, isDirected);
    initialisePathMatrix();
    printf("Initialised Graph\n");
    printAdjMatrix();

    if (verbose == true)
    {
        printPathMatrix();
    }


    //printf("York to Hull Energy = %g\n", readEdgeWeight(findCityIndex("York"), findCityIndex("Hull")));
    //printf("Leeds to York Energy = %g\n", readEdgeWeight(findCityIndex("Leeds"), findCityIndex("York")));
}

void runFloydAlgorithm(List* energyList, List* citypairsList, char* outputFileName)
{
    findShortestPath();
    printf("Shortest Path Graph\n");
    printAdjMatrix();
    if (verbose == true)
    {
        printPathMatrix();
    }

    /* If results are wanted to be printed directly after running the algorithm
    Then uncomment this to print the results to the text file */
//    ListNode* currentNode;
//    ListNode* nextNode = citypairsList->head;
//
//    while (nextNode != NULL)
//    {
//        currentNode = nextNode;
//        nextNode = currentNode->next;
//
//        printCitypair(currentNode, outputFileName);
//    }



    // Used to test multiple city pair shortest paths when writing the program.
//    if (verbose == true)
//    {
//        printf("Manchester to Perth Energy = %g\n", readEdgeWeight(findCityIndex("Manchester"), findCityIndex("Perth")));
//        printf("Liverpool to Whitby Energy = %g\n", readEdgeWeight(findCityIndex("Liverpool"), findCityIndex("Whitby")));
//        printf("Lincoln to Birmingham Energy = %g\n", readEdgeWeight(findCityIndex("Lincoln"), findCityIndex("Birmingham")));
//        printf("Liverpool to Leeds Energy = %g\n", readEdgeWeight(findCityIndex("Liverpool"), findCityIndex("Leeds")));
//        printf("Cardiff to Bristol Energy = %g\n", readEdgeWeight(findCityIndex("Cardiff"), findCityIndex("Bristol")));
//
//        printf("\n\n---4 Cities---\n\n");
//
//        printf("Leeds to York Energy = %g\n", readEdgeWeight(findCityIndex("Leeds"), findCityIndex("York")));
//        printf("York to Leeds Energy = %g\n", readEdgeWeight(findCityIndex("York"), findCityIndex("Leeds")));
//
//        printf("York to Doncaster Energy = %g\n", readEdgeWeight(findCityIndex("York"), findCityIndex("Doncaster")));
//        printf("Doncaster to York Energy = %g\n", readEdgeWeight(findCityIndex("Doncaster"), findCityIndex("York")));
//
//        printf("Leeds to Doncaster Energy = %g\n", readEdgeWeight(findCityIndex("Leeds"), findCityIndex("Doncaster")));
//        printf("Doncaster to Leeds Energy = %g\n", readEdgeWeight(findCityIndex("Doncaster"), findCityIndex("Leeds")));
//
//        printf("Leeds to Sheffield Energy = %g\n", readEdgeWeight(findCityIndex("Leeds"), findCityIndex("Sheffield")));
//        printf("Sheffield to Leeds Energy = %g\n", readEdgeWeight(findCityIndex("Sheffield"), findCityIndex("Leeds")));
//
//        printf("Sheffield to Doncaster Energy = %g\n", readEdgeWeight(findCityIndex("Sheffield"), findCityIndex("Doncaster")));
//        printf("Doncaster to Sheffield Energy = %g\n", readEdgeWeight(findCityIndex("Doncaster"), findCityIndex("Sheffield")));
//    }


}

void printCitypair(ListNode* citypairNode, char* outputFileName)
{
    float pathEnergy = readEdgeWeight(citypairNode->source, citypairNode->destination);
    printCitypairDistance(citypairNode, pathEnergy, outputFileName);
}

void printCitypairDistance(ListNode* citypairNode, float pathEnergy, char* outputFileName)
{
    FILE *outputFP;
    outputFP = fopen(outputFileName, "a");
    fprintf(outputFP, "%s to %s Energy = %g\n", returnCity(citypairNode->source),
                                                returnCity(citypairNode->destination),
                                                pathEnergy);
    fclose(outputFP);

    if (verbose == true)
    {
        printf("%s to %s Energy = %g\n", returnCity(citypairNode->source),
                                    returnCity(citypairNode->destination),
                                    pathEnergy);
    }
}


void runBellmanFord(List* energyList, List* citypairsList, char* isDirected, char* outputFileName)
{

    // create the graph given in above figure
    int pathDestinationDistance = INFINITY;
    int pathDestinationDistance2 = INFINITY;

    Graph* myGraph = graphConstructor(numOfUniqueCities, energyList->size);
    Graph* myGraph2 = NULL;

    if (strcmp(isDirected, "-vedir") == 0)
    {
        myGraph2 = graphConstructor(numOfUniqueCities, energyList->size);
        addToAdjListGraph(myGraph, energyList, citypairsList, "dir");
        addToAdjListGraph(myGraph2, energyList, citypairsList, "revdir");
    }
    else
    {
        addToAdjListGraph(myGraph, energyList, citypairsList, isDirected);
    }


    ListNode* currentPairNode;
    ListNode* nextPairNode = citypairsList->head;

    while (nextPairNode != NULL)
    {
        currentPairNode = nextPairNode;
        nextPairNode = currentPairNode->next;

        pathDestinationDistance = bellmanford(myGraph, currentPairNode->source, currentPairNode->destination);

        if (strcmp(isDirected, "-vedir") == 0)
        {
            pathDestinationDistance2 = bellmanford(myGraph2, currentPairNode->source, currentPairNode->destination);

            if (pathDestinationDistance2 < pathDestinationDistance)
            {
                pathDestinationDistance = pathDestinationDistance2;
            }

        }

        printCitypairDistance(currentPairNode, pathDestinationDistance, outputFileName);
    }

    graphDestructor(myGraph);

    if (strcmp(isDirected, "-vedir") == 0)
    {
        graphDestructor(myGraph2);
    }

}

void addToAdjListGraph(Graph* myGraph, List* energyList, List* citypairsList, char *isDirected)
{
    ListNode* currentNode;
    ListNode* nextNode = energyList->head;

    while (nextNode != NULL)
    {
        currentNode = nextNode;
        nextNode = currentNode->next;

        addEdgeAdjList(myGraph, currentNode->source, currentNode->destination, currentNode->energy, isDirected);
    }

    printGraph(myGraph);

    printf("\n\n\n");

}

