#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Queue.h"


void queueInit(Queue *q)
{
    q->sizeOfQueue = 0;
    q->head = q->tail = NULL;
}
char* removeFirst(Queue *q){
    if (q->head == NULL)
    {
        return "";
    }
    else
    {
        Node* first = q->head;
        q->head = first->next;
        if (q->sizeOfQueue == 1)
        {
            q->tail = NULL;
        }
        q->sizeOfQueue--;
        return first->data;
    }
}
int isEmpty(const Queue *q){
    return q->sizeOfQueue == 0;
}
char* getFirst(Queue *q)
{
    if (isEmpty(q))
        return "";
    else
        return q->head->data;
}
void addLast(Queue *q, const char* string){

    Node* node = (Node*)malloc(sizeof(Node));
    char* str = (char*) malloc((strlen(string)+1)* sizeof(char));
    strcpy(str, string);
    node->data = str;

    if (q->tail == NULL)
    {
        node->next = NULL;
        q->head = node;
        q->tail = node;

    }
    else
    {
        q->tail->next = node;
        q->tail = node;
    }
    q->sizeOfQueue++;
}

void printQueue(const Queue* q)
{
    Node* n = q->head;
    while (n != NULL)
    {
        printf("%s ",n->data);
        n = n->next;
    }
    printf("\n");
}


