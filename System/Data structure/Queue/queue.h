#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#define MAXQUEUE 100

typedef struct Queue {
    int front;
    int rear;
    int size;
    int elements[MAXQUEUE];  // Changed from 'array' to 'elements'
} Queue;

void createQueue(Queue *pq);
void Append(int data, Queue *pq);
void serve(int *pdata, Queue *pq);
int QueueEmpty(Queue *pq);
int QueueFull(Queue *pq);
int QueueSize(Queue *pq);
void clearQueue(Queue *pq);
void TraverseQueue(Queue *pq, void (*pf)(int));
void printElement(int element);

#endif // QUEUE_H_INCLUDED
