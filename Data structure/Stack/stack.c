#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void createStack(Stack *ps) {
    ps->top = 0;
}

int isStackEmpty(Stack *ps) {
    return ps->top == 0;
}

void push(int element, Stack *ps) {
    if (ps->top < MAXSTACK) {
        ps->elements[ps->top++] = element;
    } else {
        printf("Stack is full\n");
    }
}

int pop(Stack *ps) {
    if (!isStackEmpty(ps)) {
        return ps->elements[--ps->top];
    } else {
        printf("Stack is empty\n");
        return -1; // Return a sentinel value to indicate an error
    }
}

int firstElement(Stack *ps) {
    if (!isStackEmpty(ps)) {
        return ps->elements[0];
    } else {
        printf("Stack is empty\n");
        return -1; // Return a sentinel value to indicate an error
    }
}

void destroyStack(Stack *ps) {
    ps->top = 0; // Reset the stack
}

void copyStack(Stack *from, Stack *to) {
    for (int i = 0; i < from->top; i++) {
        to->elements[i] = from->elements[i];
    }
    to->top = from->top;
}

int stackSize(Stack *ps) {
    return ps->top;
}

void traverseStack(Stack *ps) {
    if (isStackEmpty(ps)) {
        printf("Stack is empty\n");
        return;
    }
    printf("Stack elements: ");
    for (int i = 0; i < ps->top; i++) {
        printf("%d ", ps->elements[i]);
    }
    printf("\n");
}
