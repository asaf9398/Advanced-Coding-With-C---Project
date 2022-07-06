#include "InstrumentesPriceListFuncs.h"
#include <stdlib.h>

//get an allocated list and makes it empty
void makeEmptyMPIList(MPIList* lst)
{
    lst->head = lst->tail = NULL;
}
//insert data to the end of the list by calling funcs that creates a new node and another func that insert it to the end of the list
void insertDataToEndMPIList(MPIList* lst, unsigned short id, float price)
{
    MPIListNode* newTail;
    newTail = createNewMPIListNode(id, price, NULL);
    insertNodeToEndMPIList(lst, newTail);
}
//creates a new node
MPIListNode* createNewMPIListNode(unsigned short id, float price, MPIListNode* next)
{
    MPIListNode* res;
    res = (MPIListNode*)malloc(sizeof(MPIListNode));
    checkMemoryAllocation(res);
    res->instrument.insId = id;
    res->instrument.price = price;
    res->next = next;
    return res;
}
//insert a given node to the end of the list
void insertNodeToEndMPIList(MPIList* lst, MPIListNode* newTail)
{
    if (isEmptyMPIList(lst))
    {
        lst->head = lst->tail = newTail;
    }
    else
    {
        lst->tail->next = newTail;
        lst->tail = newTail;
    }
}
//checks if a given list is empty
bool isEmptyMPIList(MPIList* list)
{
    return(list->head == NULL);
}

//free MPI list
void freeMPIList(MPIList* list)
{
    MPIListNode* res;

    while (list->head != NULL)
    {
        res = list->head;
        list->head = list->head->next;
        free(res);
    }
}