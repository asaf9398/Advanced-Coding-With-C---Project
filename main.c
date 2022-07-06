#include "treefuncs.h"
#include "InstrumentesPriceListFuncs.h"
#include "MusiciansGroup.h"
#include "concert.h"

#define _CRT_SECURE_NO_WARNINGS

void main(int argc, char* argv[])
{
    int numOfInstruments, numOfMusicians;
    Musician** musicianGroup;
    Musician*** musiciansCollection;
    InstrumentTree tr;
    char** musiciansScrumbled;
    char** concertArr;
    int numOfConcerts;
    int* musiciansCollectionsSize;


    tr = BuildTreeFromFile(argv[1], &numOfInstruments);

    musiciansCollectionsSize = (int*)malloc(numOfInstruments * sizeof(int));
    checkMemoryAllocation(musiciansCollectionsSize);

    musiciansScrumbled = howManyMusicians(argv[2], &numOfMusicians);
    musicianGroup = makeMusicianArray(musiciansScrumbled, numOfMusicians, tr);

    musiciansCollection = makeMusicianCollection(numOfInstruments, musicianGroup, numOfMusicians, musiciansCollectionsSize);

    printf("please enter wanted concerts:\n");
    //the program will get all of the concerts and than will print the output for every concert

    concertArr = buildConcertArray(&numOfConcerts);

    makeConcert(concertArr, numOfConcerts, tr, musiciansCollection, musiciansCollectionsSize, numOfInstruments);

    freeConcertArr(concertArr, numOfConcerts);

    freeMusiciansGroup(musicianGroup, numOfMusicians);

    freeMusiciansCollection(musiciansCollection, numOfInstruments);

    freeTree(tr);
}
