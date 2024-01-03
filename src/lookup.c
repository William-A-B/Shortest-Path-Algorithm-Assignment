#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "fileio.h"
#include "lookup.h"



typedef struct Array Array;

struct Array
{
    int capacity;
    int numEntities;
    int *entities;
};


//enum {monday, tuesday, wednesday};


//void sort(List* list)
//{
//    int index = 0;
//    char tmpCity[20];
//    Array* array;
//
//    while (index < listSize(list))
//    {
//        tmpCity = list->head->startCity;
//
//        for (int i = 0; i < array->numEntities; i++)
//        {
//            if (strcmp(tmpCity, array->entities[i]) == 0)
//                break;
//            else
//            {
//                array->entities[i] = tmpCity;
//            }
//        }
//
//        if (list->head->startCity)
//    }
//}
