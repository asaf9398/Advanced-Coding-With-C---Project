
#ifndef FINALPROJECT_MUSICIANSGROUP_H
#define FINALPROJECT_MUSICIANSGROUP_H
#define _CRT_SECURE_NO_WARNINGS
#include "InstrumentesPriceListFuncs.h"
#include "treefuncs.h"
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char** name;
    int numOfNames;
    bool selected;
    MPIList instruments;
} Musician;

typedef struct musucianListNode
{
    char** name;
    int numOfNames;
    char* instrument;
    float price;
    struct musucianListNode* next;
} MusicianListNode;

typedef struct musicianList
{
    MusicianListNode* head;
    MusicianListNode* tail;
} MusicianList;

Musician** makeMusicianArray(char** arr, int size, InstrumentTree tr);

char** howManyMusicians(char* fileName, int* size);

void printArr(Musician** arr, int size);

void freeMusiciansGroup(Musician** arr, int size);

void freeNameArray(char** arr, int size);

Musician*** makeMusicianCollection(int numOfInstruments, Musician** MusicianGroup, int musicianGroupSize, int* musiciansCollectionsSizes);

Musician** findMusiciansByInstrumentID(int instrumentID, Musician** MusicianGroup, int musicianGroupSize, int* musiciansCollectionsSizes);

bool isInstrumentOnList(int instrumentId, MPIList instrumentList);

void freeMusiciansCollection(Musician*** arr, int size);

float findPrice(Musician musician, int instId);

void makeEmptyMusicianList(MusicianList* lst);
void insertDataToEndMusicianList(MusicianList* lst, char** name, int numOfNames, char* instrument, float price, MusicianListNode* next);
MusicianListNode* createNewMusicianListNode(char** name, int numOfNames, char* instrument, float price, MusicianListNode* next);
void insertNodeToEndMusicianList(MusicianList* lst, MusicianListNode* newTail);
bool isEmptyMusicianList(MusicianList* list);
void freeMusicianList(MusicianList* list);
void printMusicianList(MusicianList lst);


void printCollection(Musician** arr, int size);

#endif //FINALPROJECT_MUSICIANSGROUP_H




//the number of musicians will be available from the func that build the first array from the file