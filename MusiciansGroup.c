#include "MusiciansGroup.h"

//get strings array of "illegal"  string and an instruments tree,
//and creates an array of pointers to musicians, in which every musician has a list of the instruments he plays
//and the price of every instrument, and a strings array of their names.
Musician** makeMusicianArray(char** arr, int size, InstrumentTree tr)
{
    char copyName[150];
    int row;
    int idRes;
    float priceRes;
    int logSize, phySize;
    Musician** pMusicianArr;
    char seps[] = " ,.;:?!-\t'()[]{}<>~_";

    pMusicianArr = (Musician**)malloc(size * sizeof(Musician*));
    checkMemoryAllocation(pMusicianArr);


    for (row = 0; row < size; row++)
    {
        logSize = 0, phySize = 2;
        pMusicianArr[row] = (Musician*)malloc(sizeof(Musician));
        checkMemoryAllocation(pMusicianArr[row]);

        pMusicianArr[row]->selected = false;

        makeEmptyMPIList(&(pMusicianArr[row]->instruments));

        pMusicianArr[row]->name = (char**)malloc(phySize * sizeof(char*));
        checkMemoryAllocation(pMusicianArr[row]->name);

        char* token;
        strcpy(copyName, arr[row]);

        token = strtok(copyName, seps);

        while (token != NULL)
        {
            idRes = findInsId(tr, token);

            if (idRes == -1)
            {
                if (logSize == phySize)
                {
                    phySize *= 2;
                    pMusicianArr[row]->name = (char**)realloc(pMusicianArr[row]->name, phySize * sizeof(char*));
                    checkMemoryAllocation(pMusicianArr[row]->name);
                }

                pMusicianArr[row]->name[logSize] = _strdup(token);
                logSize++;

            }
            else // it is an instrument followed by its price
            {
                token = strtok(NULL, seps);
                sscanf(token, "%f", &priceRes);
                insertDataToEndMPIList(&(pMusicianArr[row]->instruments), idRes, priceRes);
            }
            token = strtok(NULL, seps);
        }

        if (logSize != phySize)
        {
            pMusicianArr[row]->name = (char**)realloc(pMusicianArr[row]->name, logSize * sizeof(char*));
            checkMemoryAllocation(pMusicianArr[row]->name);
        }
        pMusicianArr[row]->numOfNames = logSize;
    }
    freeNameArray(arr, size);
    return pMusicianArr;
}
//creates array of strings of musicians from the given file
char** howManyMusicians(char* fileName, int* size)
{
    unsigned long tempLen;
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("Error , file opening failed!\n");
        exit(1);
    }
    int logSize = 0, phySize = 1;
    char** arr;
    arr = (char**)malloc(phySize * sizeof(char*));
    checkMemoryAllocation(arr);
    char temp[MAX_STR_LEN + 1];

    while (!feof(fp))
    {
        fgets(temp, MAX_STR_LEN + 1, fp);
        tempLen = strlen(temp);
        if (temp[tempLen - 1] == '\n')
            temp[tempLen - 1] = '\0';
        if (logSize == phySize)
        {
            phySize *= 2;
            arr = (char**)realloc(arr, phySize * sizeof(char*));
            checkMemoryAllocation(arr);
        }
        arr[logSize] = _strdup(temp);
        logSize++;
    }
    fclose(fp);
    arr = (char**)realloc(arr, logSize * sizeof(char*));
    checkMemoryAllocation(arr);
    *size = logSize;
    return arr;
}


//get the number of instruments and the musicians array and its size,
//it creates array of arrays of pointers to musicians that plays on the curr index of the array instrument id
Musician*** makeMusicianCollection(int numOfInstruments, Musician** MusicianGroup, int musicianGroupSize, int* musiciansCollectionsSizes)
{
    int i;
    Musician*** musiciansCollection;



    musiciansCollection = (Musician***)malloc(numOfInstruments * sizeof(Musician**));

    checkMemoryAllocation(musiciansCollection);

    for (i = 0; i < numOfInstruments; i++)
    {
        musiciansCollection[i] = findMusiciansByInstrumentID(i, MusicianGroup, musicianGroupSize, &musiciansCollectionsSizes[i]);
    }

    return musiciansCollection;
}

//get a specific instrument id , the musicians array, and its size.
//and returns an array of pointers to musicians that play this instrument
Musician** findMusiciansByInstrumentID(int instrumentID, Musician** MusicianGroup, int musicianGroupSize, int* musiciansCollectionsSizes)
{
    int logSize = 0, phySize = 1;
    int i;

    Musician** res;

    res = (Musician**)malloc(phySize * sizeof(Musician*));
    checkMemoryAllocation(res);

    for (i = 0; i < musicianGroupSize; i++)
    {
        if (logSize == phySize)
        {
            phySize *= 2;
            res = (Musician**)realloc(res, phySize * sizeof(Musician*));
            checkMemoryAllocation(res);
        }


        if (isInstrumentOnList(instrumentID, MusicianGroup[i]->instruments))
        {
            res[logSize] = MusicianGroup[i];
            logSize++;
        }
    }

    *musiciansCollectionsSizes = logSize;
    return res;
}

//returns true if a musician is playing on given instrument else return false
bool isInstrumentOnList(int instrumentId, MPIList instrumentList)
{
    MPIListNode* res;

    res = instrumentList.head;

    while (res != NULL)
    {
        if (res->instrument.insId == instrumentId)
            return true;
        else
            res = res->next;
    }
    return false;
}


//free musician ** arr
void freeMusiciansGroup(Musician** arr, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        freeNameArray(arr[i]->name, arr[i]->numOfNames);
        freeMPIList(&arr[i]->instruments);
        free(arr[i]);
    }
    free(arr);
}
//free the names array
void freeNameArray(char** arr, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

//free musician collection (array of arrays)
void freeMusiciansCollection(Musician*** arr, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}



//make empty musician list
void makeEmptyMusicianList(MusicianList* lst)
{
    lst->head = NULL;
    lst->tail = NULL;
}

//add musician to end list by inserting musician data
void insertDataToEndMusicianList(MusicianList* lst, char** name, int numOfNames, char* instrument, float price, MusicianListNode* next)
{
    MusicianListNode* node;
    node = createNewMusicianListNode(name, numOfNames, instrument, price, next);
    insertNodeToEndMusicianList(lst, node);
}

//creates new musician list node
MusicianListNode* createNewMusicianListNode(char** name, int numOfNames, char* instrument, float price, MusicianListNode* next)
{
    MusicianListNode* node;
    node = (MusicianListNode*)malloc(sizeof(MusicianListNode));
    checkMemoryAllocation(node);
    node->name = name;
    node->numOfNames = numOfNames;
    node->instrument = (char*)malloc((strlen(instrument) + 1) * sizeof(char));
    checkMemoryAllocation(node->instrument);
    strcpy(node->instrument, instrument);
    node->price = price;
    node->next = next;
    return node;
}

//add new tail to the list
void insertNodeToEndMusicianList(MusicianList* lst, MusicianListNode* newTail)
{
    if (isEmptyMusicianList(lst))
    {
        lst->head = lst->tail = newTail;
    }
    else
    {
        lst->tail->next = newTail;
        lst->tail = newTail;
    }
}

//checking if the list is empty
bool isEmptyMusicianList(MusicianList* list)
{
    return(list->head == NULL);
}

//free musician list
void freeMusicianList(MusicianList* list)
{
    MusicianListNode* res;

    while (list->head != NULL)
    {
        res = list->head;
        list->head = list->head->next;
        free(res->instrument);
        free(res);
    }
}
//prints musician list
void printMusicianList(MusicianList lst)
{
    MusicianListNode* res;
    res = lst.head;
    int i;
    int totalCost = 0;
    while (res != NULL)
    {
        for (i = 0; i < res->numOfNames; i++)
        {
            printf("%s ", res->name[i]);
        }
        printf("- ");
        totalCost += (int)res->price;
        if (res->next == NULL)
        {
            printf("%s (%d). ", res->instrument, (int)res->price);
        }
        else
            printf("%s (%d), ", res->instrument, (int)res->price);

        res = res->next;
    }
    printf("Total cost: %d.", totalCost);
}
//func that receive musician and id and returns price
float findPrice(Musician musician, int instId)
{
    MPIListNode* res;
    float price;
    bool found = false;
    res = musician.instruments.head;

    while (res != NULL && found == false)
    {
        if (res->instrument.insId == instId)
        {
            price = res->instrument.price;
            found = true;
        }
        res = res->next;
    }
    return price;
}
