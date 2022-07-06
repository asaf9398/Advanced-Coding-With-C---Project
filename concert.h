
#ifndef FINALPROJECT_CONCERT_H
#define FINALPROJECT_CONCERT_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "treefuncs.h"
#include "MusiciansGroup.h"

#define IMPORTANT '1'
#define MAX_INST_LEN 151

typedef struct
{
    Musician* musician;
    int instId;
}QsortHelper;


typedef struct
{
    int day, month, year;
    float hour;
}  Date;

typedef struct
{
    int num;
    int inst;
    char importance;
} ConcertInstrument;

typedef struct cilistnode
{
    ConcertInstrument inst;
    struct cilistnode* next;
} CIListNode;

typedef struct cilist
{
    CIListNode* head;
    CIListNode* tail;
}CIList;


typedef struct
{
    Date date_of_concert;
    char* name;
    CIList instruments; // ConcertInstrument
} Concert;


char** buildConcertArray(int* size);
char* getSingleConcert();
void makeConcert(char** concertStringsArray, int numOfConcerts, InstrumentTree tr, Musician*** musicianCollection, int* size, int numOfInstruments);
Concert  createSingleConcert(char* str, InstrumentTree tr);
int concertNameLen(char* str);

CIList createCIlist(char* str, InstrumentTree tr);

void printConcert(Concert wantedConcert, Musician*** musicianCollection, int* size, InstrumentTree tr, int numOfInstruments);
bool isThereEnoughInst(int* size, Concert cons);
void invalidConcert(char* name);
bool isConcertValid(Concert conc, Musician*** musicianCollection, int* size, MusicianList* musiciansList, InstrumentTree tr);
void insertDataToEndCILList(CIList* lst, int amount, int id, char importance);
void makeEmptyCILList(CIList* lst);
CIListNode* createNewCILListNode(int num, int id, char importance, CIListNode* next);
void insertNodeToEndCILList(CIList* lst, CIListNode* newTail);
bool isEmptyCILList(CIList* list);
void freeCILList(CIList* list);
void changeSelectedMusicianToFalse(Musician*** musicianCollection, int* size, int numOfInstruments);
void freeConcertArr(char** arr, int size);
int comperatorMaxToMin(void* musician1, void* musician2);
int comperatorMinToMax(void* musician1, void* musician2);
float findInstPrice(Musician m1);
QsortHelper* copyMusicianCollectionLine(Musician** arr, int size, int instId);


#endif //FINALPROJECT_CONCERT_H


