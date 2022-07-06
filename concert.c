#include "concert.h"


//builds the concerts strings array by the input from the user
char** buildConcertArray(int* size)
{
    char** concertArr;
    int logSize = 0, phySize = 1;
    bool endOfInput = false;

    concertArr = (char**)malloc(phySize * sizeof(char*));
    checkMemoryAllocation(concertArr);

    while (endOfInput != true)
    {
        if (logSize == phySize)
        {
            phySize *= 2;
            concertArr = (char**)realloc(concertArr, phySize * sizeof(char*));
            checkMemoryAllocation(concertArr);
        }
        concertArr[logSize] = _strdup(getSingleConcert());
        logSize++;

        if (strlen(concertArr[logSize - 1]) == 0)
        {
            endOfInput = true;
        }
    }
    *size = logSize - 1;
    return concertArr;
}

//gets a single string from the user
char* getSingleConcert()
{
    char* res;
    int logSize = 0, phySize = 1;
    bool isEnter = false;

    res = (char*)malloc(phySize * sizeof(char));
    checkMemoryAllocation(res);


    while (isEnter != true)
    {
        if (logSize == phySize)
        {
            phySize *= 2;
            res = (char*)realloc(res, sizeof(char) * phySize);
            checkMemoryAllocation(res);
        }
        res[logSize] = (char)getchar();
        logSize++;
        if (res[logSize - 1] == '\n')
            isEnter = true;
    }

    res[logSize - 1] = '\0';

    return res;
}
//creates a Concert array
void makeConcert(char** concertStringsArray, int numOfConcerts, InstrumentTree tr, Musician*** musicianCollection, int* size, int numOfInstruments)
{
    Concert temp;
    int i;

    for (i = 0; i < numOfConcerts; i++)
    {
        temp = createSingleConcert(concertStringsArray[i], tr);
        printConcert(temp, musicianCollection, size, tr, numOfInstruments);
        free(temp.name);
        freeCILList(&temp.instruments);
    }

}
//creates single concert
Concert createSingleConcert(char * str, InstrumentTree tr)
{
    Concert res;
    float min;

    int concertNameLength = concertNameLen(str);

    res.name = (char*)malloc(concertNameLength * sizeof(char));
    checkMemoryAllocation(res.name);

    sscanf(str, "%s %d %d %d %f:%f", res.name, &res.date_of_concert.day, &res.date_of_concert.month, &res.date_of_concert.year, &res.date_of_concert.hour, &min);

    res.date_of_concert.hour += min / 100;

    res.instruments = createCIlist(str + concertNameLength + 17, tr);

    return res;
}

//creates CI list from given string
CIList createCIlist(char* str, InstrumentTree tr)
{
    CIList res;
    makeEmptyCILList(&res);
    char seps[] = " ";
    char* instrumentName;
    char tempName[MAX_INST_LEN];
    int instId;
    int amount;
    char importance;
    char* token;

    int counter = 1;
    token = strtok(str, seps);

    while (token != NULL)
    {
        if (counter % 3 == 1)
        {
            //its the inst name
            strcpy(tempName, token);
            instrumentName = _strdup(tempName);
            instId = findInsId(tr, instrumentName);
            free(instrumentName);
            token = strtok(NULL, seps);
            counter++;
        }
        if (counter % 3 == 2)
        {
            //its num of instrument
            sscanf(token, "%d", &amount);
            token = strtok(NULL, seps);
            counter++;
        }
        if (counter % 3 == 0)
        {
            //its importance
            sscanf(token, "%c", &importance);
            token = strtok(NULL, seps);
            counter++;
        }

        insertDataToEndCILList(&res, amount, instId, importance);
    }

    return res;
}
//if concert is valid it prints it by the template
void printConcert(Concert wantedConcert, Musician*** musicianCollection, int* size, InstrumentTree tr, int numOfInstruments)
{
    MusicianList currConcertMusicians;
    makeEmptyMusicianList(&currConcertMusicians);

    if (!isConcertValid(wantedConcert, musicianCollection, size, &currConcertMusicians, tr))
        invalidConcert(wantedConcert.name);
    else
    {
        printf("%s ", wantedConcert.name);
        printf("%02d %02d %d ", wantedConcert.date_of_concert.day, wantedConcert.date_of_concert.month, wantedConcert.date_of_concert.year);
        float time = wantedConcert.date_of_concert.hour;

        time *= 100;
        printf("%02d:%02d: ", (int)time / 100, (int)time % 100);
        printMusicianList(currConcertMusicians);
        printf("\n");
    }

    freeMusicianList(&currConcertMusicians);
    changeSelectedMusicianToFalse(musicianCollection, size, numOfInstruments);
}
//checks if given concert is valid
bool isConcertValid(Concert conc, Musician*** musicianCollection, int* size, MusicianList* musiciansList, InstrumentTree tr)
{
    QsortHelper* currInst;
    CIListNode* curr;
    int curr_Ins_Id;
    int i, j;
    int tempNum;

    if (!isThereEnoughInst(size, conc))//there are not enough inst to make concert
        return false;

    curr = conc.instruments.head;

    while (curr != NULL)
    {
        curr_Ins_Id = curr->inst.inst;

        currInst = copyMusicianCollectionLine(musicianCollection[curr_Ins_Id], size[curr_Ins_Id], curr_Ins_Id);

        if (curr->inst.importance == IMPORTANT)
        {
            qsort(currInst, size[curr_Ins_Id], sizeof(QsortHelper), comperatorMaxToMin);
        }
        else
        {
            qsort(currInst, size[curr_Ins_Id], sizeof(QsortHelper), comperatorMinToMax);
        }


        for (i = 0; i < size[curr_Ins_Id]; i++)
        {
            musicianCollection[curr_Ins_Id][i] = currInst[i].musician;
        }
        free(currInst);

        i = 0, j = 0;
        tempNum = curr->inst.num;

        while (i < tempNum && j < size[curr->inst.inst])
        {
            if (musicianCollection[curr->inst.inst][j]->selected == false)
            {
                char** musicianName = musicianCollection[curr->inst.inst][j]->name;
                int numOfNames = musicianCollection[curr->inst.inst][j]->numOfNames;
                char* instrument = findInstName(curr->inst.inst, tr);
                float price = findPrice(*musicianCollection[curr->inst.inst][j], curr->inst.inst);

                insertDataToEndMusicianList(musiciansList, musicianName, numOfNames, instrument, price, NULL);
                musicianCollection[curr->inst.inst][j]->selected = true;
                j++;
                i++;
            }
            else
            {
                j++;
            }
        }
        if (i != tempNum)
            return false;

        curr = curr->next;
    }
    return true;
}
//if concert is invalid it will print message to the user
void invalidConcert(char* name)
{
    printf("Could not find musicians for the concert %s\n", name);
}
//it checks if there are enough instruments from a wanted instrument
bool isThereEnoughInst(int* size, Concert cons)
{

    CIListNode* curr;
    curr = cons.instruments.head;
    while (curr != NULL)
    {
        if (size[curr->inst.inst] < curr->inst.num)
            return false;

        curr = curr->next;
    }
    return true;
}

//make a list empty
void makeEmptyCILList(CIList* lst)
{
    lst->head = lst->tail = NULL;
}
//insert data to the end of CI list
void insertDataToEndCILList(CIList* lst, int amount, int id, char importance)
{
    CIListNode* newTail;
    newTail = createNewCILListNode(amount, id, importance, NULL);
    insertNodeToEndCILList(lst, newTail);
}
//creates new CI list node
CIListNode* createNewCILListNode(int num, int id, char importance, CIListNode* next)
{
    CIListNode* res;
    res = (CIListNode*)malloc(sizeof(CIListNode));
    checkMemoryAllocation(res);
    res->inst.num = num;
    res->inst.inst = id;
    res->inst.importance = importance;
    res->next = next;
    return res;
}
//insert CI list node to the end of given list
void insertNodeToEndCILList(CIList* lst, CIListNode* newTail)
{
    if (isEmptyCILList(lst))
    {
        lst->head = lst->tail = newTail;
    }
    else
    {
        lst->tail->next = newTail;
        lst->tail = newTail;
    }
}
//checks if given list is empty
bool isEmptyCILList(CIList* list)
{
    return(list->head == NULL);
}
//free CI list
void freeCILList(CIList* list)
{
    CIListNode* res;

    while (list->head != NULL)
    {
        res = list->head;
        list->head = list->head->next;
        free(res);
    }
}
//free concert arr (char**)
void freeConcertArr(char** arr, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}
//checks for the length of concert
int concertNameLen(char* str)
{
    int i = 0;
    bool isCharSpace = false;

    while (!isCharSpace)
    {
        if (str[i] == ' ')
            isCharSpace = true;
        i++;
    }
    return i;
}
//in the end of every concert making, it returns every musician to unselected
void changeSelectedMusicianToFalse(Musician*** musicianCollection, int* size, int numOfInstruments)
{
    int i, j;

    for (i = 0; i < numOfInstruments; i++)
    {
        for (j = 0; j < size[i]; j++)
        {
            musicianCollection[i][j]->selected = false;
        }
    }

}

//if we want to sort the array max to min , we will call qsort with this func
int comperatorMaxToMin(void* musician1, void* musician2)
{
    QsortHelper* m1 = (QsortHelper*)musician1;

    QsortHelper* m2 = (QsortHelper*)musician2;

    float m1Price = findPrice(*m1->musician, m1->instId);
    float m2Price = findPrice(*m2->musician, m2->instId);
    int diff = (m1Price - m2Price);

    if (diff > 0)
        return -1;
    else
        return 1;

}
//if we want to sort the array min to max , we will call qsort with this func
int comperatorMinToMax(void* musician1, void* musician2)
{
    QsortHelper* m1 = (QsortHelper*)musician1;
    QsortHelper* m2 = (QsortHelper*)musician2;

    float m1Price = findPrice(*m1->musician, m1->instId);
    float m2Price = findPrice(*m2->musician, m2->instId);
    int diff = (m1Price - m2Price);

    if (diff > 0)
        return 1;
    else
        return -1;
}

//copys a line form musician collections with the instid the original line represents
QsortHelper* copyMusicianCollectionLine(Musician** arr, int size, int instId)
{
    int i;
    QsortHelper* res;

    res = (QsortHelper*)malloc(size * sizeof(QsortHelper));
    checkMemoryAllocation(res);

    for (i = 0; i < size; i++)
    {
        res[i].musician = arr[i];
        res[i].instId = instId;
    }

    return res;
}