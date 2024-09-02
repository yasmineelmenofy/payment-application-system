#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#define MAXSTACK 100

typedef struct Stack {
    int elements[MAXSTACK]; // Renamed to avoid conflicts
    int top;
} Stack;

void createStack(Stack *ps);
int isStackEmpty(Stack *ps);
void push(int element, Stack *ps);
int pop(Stack *ps);
int firstElement(Stack *ps);
void destroyStack(Stack *ps);
void copyStack(Stack *from, Stack *to);
int stackSize(Stack *ps);
void traverseStack(Stack *ps);




#endif // STACK_H_INCLUDED
