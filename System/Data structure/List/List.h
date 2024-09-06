#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include "File handling.h"



// Define the node structure using the union
typedef struct listnode {
    RecordData entry;             // Use the union for storing either type
    struct listnode *next;       // Pointer to the next node
} listnode;

// Define the list structure
typedef struct List {
    listnode *head;              // Pointer to the head node
    int size;                    // Current size of the list
} List;

void initList(List *pl);
void deleteList(int pos, RecordData *pe, List *pl);
void insertList(int pos, RecordData e, List *pl);
void traverseList(List *pl, void (*visit)(RecordData));
void freeList(List *pl);
int retrieveList(int pos, RecordData *pe, List *pl);
void replaceList(int pos, RecordData e, List *pl);
int isEmptyList(List *pl);
int isFullList(List *pl);
int listSize(List *pl);


#endif // LIST_H_INCLUDED
