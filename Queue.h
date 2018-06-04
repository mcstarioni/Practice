//
// Created by mcstarioni on 04.06.2018.
//
#ifndef PRACTICS_QUEUE_H
#define PRACTICS_QUEUE_H

#endif //PRACTICS_QUEUE_H
typedef struct QueueNode
{
    char* data;
    struct QueueNode *next;
} Node;

typedef struct QueueList
{
    int sizeOfQueue;
    Node *head;
    Node *tail;
} Queue;


void queueInit(Queue *q);
char* removeFirst(Queue *q);
int isEmpty(const Queue *q);
char* getFirst(Queue *q);
void addLast(Queue *q, const char* string);

void printQueue(const Queue* q);