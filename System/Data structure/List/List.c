#include <stdio.h>
#include <stdlib.h>
#include "List.h"

// Initialize the list
void initList(List *pl) {
    pl->head = NULL;
    pl->size = 0;
}

// Delete an entry at a specific position
void deleteList(int pos, RecordData *pe, List *pl) {
    if (pos < 0 || pos >= pl->size) {
        printf("Error: Position out of bounds.\n");
        return;
    }

    listnode *temp;
    if (pos == 0) {
        *pe = pl->head->entry; // Get the entry before deleting
        temp = pl->head;
        pl->head = pl->head->next;
        free(temp);
    } else {
        listnode *q = pl->head;
        for (int i = 0; i < pos - 1; i++) {
            q = q->next;
        }
        *pe = q->next->entry; // Get the entry before deleting
        temp = q->next;
        q->next = temp->next;
        free(temp);
    }
    pl->size--;
}

// Insert a new entry at a specific position
void insertList(int pos, RecordData e, List *pl) {
    listnode *p, *q;
    int i;

    p = (listnode*)malloc(sizeof(listnode));
    if (p == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    p->entry = e;
    p->next = NULL;

    if (pos == 0) {
        p->next = pl->head;
        pl->head = p;
    } else {
        for (q = pl->head, i = 0; i < pos - 1; i++) {
            q = q->next;
        }
        p->next = q->next;
        q->next = p;
    }
    pl->size++;
}

// Traverse the list and apply a function to each entry
void traverseList(List *pl, void (*visit)(RecordData)) {
    listnode *p = pl->head;
    while (p) {
        visit(p->entry); // Call visit function with the union data
        p = p->next;
    }
}

// Free all entries in the list
void freeList(List *pl) {
    listnode *current = pl->head;
    listnode *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    pl->head = NULL;
    pl->size = 0;
}

// Retrieve an entry at a specific position
int retrieveList(int pos, RecordData *pe, List *pl) {
    if (pos < 0 || pos >= pl->size) {
        printf("Error: Position out of bounds.\n");
        return 0; // Failure
    }

    listnode *q = pl->head;
    for (int i = 0; i < pos; i++) {
        q = q->next;
    }
    *pe = q->entry; // Retrieve the entry
    return 1; // Success
}

// Replace an entry at a specific position
void replaceList(int pos, RecordData e, List *pl) {
    if (pos < 0 || pos >= pl->size) {
        printf("Error: Position out of bounds.\n");
        return;
    }

    listnode *q = pl->head;
    for (int i = 0; i < pos; i++) {
        q = q->next;
    }
    q->entry = e; // Replace the entry
}

// Check if the list is empty
int isEmptyList(List *pl) {
    return pl->size == 0;
}

// Check if the list is full (never full for linked lists)
int isFullList(List *pl) {
    return 0; // A linked list is never "full" unless you run out of memory
}

// Get the size of the list
int listSize(List *pl) {
    return pl->size;
}

