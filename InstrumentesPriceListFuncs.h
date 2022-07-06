
#ifndef FINALPROJECT_INSTRUMENTESPRICELISTFUNCS_H
#define FINALPROJECT_INSTRUMENTESPRICELISTFUNCS_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include "treefuncs.h"
typedef struct
{
    unsigned short insId;
    float price;
} MusicianPriceInstrument;

typedef struct list_node
{
    MusicianPriceInstrument instrument;
    struct list_node* next;
}MPIListNode;

typedef struct list
{
    MPIListNode* head;
    MPIListNode* tail;
}MPIList;

void makeEmptyMPIList(MPIList* lst);
void insertDataToEndMPIList(MPIList* lst, unsigned short id, float price);
MPIListNode* createNewMPIListNode(unsigned short id, float price, MPIListNode* next);
void insertNodeToEndMPIList(MPIList* lst, MPIListNode* newTail);
bool isEmptyMPIList(MPIList* list);
void freeMPIList(MPIList* list);

#endif //FINALPROJECT_INSTRUMENTESPRICELISTFUNCS_H
